def changes

pipeline {
    agent any
    environment {
        GITHUB_GHCR_PAT = credentials('github_pat_packages')
        TOKEN_TA_NOTIFIER = credentials('my_ta_notifier_api')
    }
    stages {
        stage('Build, Publish, Deploy Docusaurus') {
            when {
                branch 'main'
            }
            stages {
                stage('Check Changes') {
                    steps {
                        script {
                            changes = sh(
                                script: "git diff-tree --no-commit-id --name-only -r HEAD | grep '^documentation/docusaurus/' || true",
                                returnStdout: true
                            ).trim()
                            if (!changes) {
                                echo "No changes detected in /documentation/docusaurus, ignoring Docker build."
                            } else {
                                echo "Changes detected in /documentation/docusaurus :"
                                echo changes
                            }
                        }
                    }
                }
                stage('Docker Build and Publish') {
                    when {
                        expression { (!!changes) == true }
                    }
                    stages {
                        stage('Build Docker Image') {
                            steps {
                                script {
                                    sh '''
                                        cd documentation/docusaurus
                                        docker build -t rtype-documentation:latest .
                                    '''
                                }
                            }
                        }
                        stage('Push Docker Image') {
                            steps {
                                script {
                                    sh 'echo $GITHUB_GHCR_PAT | docker login ghcr.io -u a9ex --password-stdin'
                                    sh 'docker tag rtype-documentation:latest ghcr.io/epitechpromo2027/rtype-documentation:latest'
                                    sh 'docker push ghcr.io/epitechpromo2027/rtype-documentation:latest'
                                }
                            }
                        }
                    }
                }
                stage('Notify Server - Deploy docs') {
                    when {
                        expression { (!!changes) == true }
                    }
                    steps {
                        script {
                            sh 'curl --fail-with-body --header "Authorization: Bearer $TOKEN_TA_NOTIFIER" https://mytanotifier.a1ex.fr/api/build-rtype-docs'
                        }
                    }
                }
                stage('Docker logout') {
                    steps {
                        script {
                            sh 'docker logout ghcr.io'
                        }
                    }
                }
            }
        }
        stage('Build and Publish Binaries') {
            parallel {
                stage('Linux') {
                    agent {
                        docker {
                            image 'ghcr.io/a9ex/epitech-devcontainer@sha256:3222291beff662c9570eff60887c0d8e0cf02e4e26f8f4f58f91cd7120095fa4'
                            args '-u root'
                        }
                    }
                    stages {
                        stage('Install conan') {
                            steps {
                                script {
                                    sh '''#!/bin/bash
                                        make clean
                                        make conan_ci
                                    '''
                                }
                            }
                        }
                        stage('Install deps') {
                            steps {
                                script {
                                    sh '''#!/bin/bash
                                        source rtype_venv/bin/activate
                                        make deps
                                    '''
                                }
                            }
                        }
                        stage('Build binaries') {
                            steps {
                                script {
                                    sh '''#!/bin/bash
                                        make
                                    '''
                                }
                            }
                        }
                        stage('Archive artifacts') {
                            steps {
                                archiveArtifacts artifacts: 'build/client/r-type_*', fingerprint: true
                                archiveArtifacts artifacts: 'build/server/r-type_*', fingerprint: true
                            }
                        }
                    }
                }
                stage('Windows') {
                    agent {
                        docker {
                            image 'ghcr.io/a9ex/ubuntu-24-mingw:latest'
                            args '-u root'
                        }
                    }
                    stages {
                        stage('Install conan') {
                            steps {
                                script {
                                    sh '''#!/bin/bash
                                        make clean
                                        make conan_ci
                                    '''
                                }
                            }
                        }
                        stage('Install deps') {
                            steps {
                                script {
                                    sh '''#!/bin/bash
                                        source rtype_venv/bin/activate
                                        make deps_windows_release
                                    '''
                                }
                            }
                        }
                        stage('Build exe') {
                            steps {
                                script {
                                    sh '''#!/bin/bash
                                        make
                                    '''
                                }
                            }
                        }
                        stage('Archive artifacts') {
                            steps {
                                archiveArtifacts artifacts: 'build/client/r-type_*', fingerprint: true
                                archiveArtifacts artifacts: 'build/server/r-type_*', fingerprint: true
                            }
                        }
                    }
                }
            }
        }

        stage('Create GitHub Release') {
            steps {
                script {
                    def version = sh(
                        script: "sed -n 's/project(rtype VERSION \([^)]*\))/\1/p' CMakeLists.txt",
                        returnStdout: true
                    ).trim()

                    if (!version) {
                        error "Could not extract version from CMakeLists.txt"
                    }

                    def tag = "v${version}"
                    def repo = "444leod/R-Type"

                    withCredentials([string(credentialsId: 'github_pat_packages', variable: 'GITHUB_TOKEN')]) {
                        sh """
                            if ! command -v gh &> /dev/null; then
                                sudo apt-get update
                                sudo apt-get install -y gh
                            fi

                            echo "\$GITHUB_TOKEN" | gh auth login --with-token

                            if gh release view ${tag} --repo github.com/${repo} &> /dev/null; then
                                gh release delete ${tag} -y --repo github.com/${repo}
                                git push origin :refs/tags/${tag} || true
                            fi

                            gh release create ${tag} \\
                                --repo github.com/${repo} \\
                                --title "R-Type ${version}" \\
                                --notes "Release \${version}" \\
                                \$(find ./build -name "r-type_*" -type f)

                            gh auth logout
                        """
                    }
                }
            }
        }
    }
    post {
        always {
            sh 'sudo chmod -R 777 .'
            cleanWs()
            echo "Pipeline OK"
        }
        failure {
            echo "Pipeline KO"
        }
    }
}
