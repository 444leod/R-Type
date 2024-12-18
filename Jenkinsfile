def changes

pipeline {
    agent any
    environment {
        GITHUB_GHCR_PAT = credentials('github_pat_packages')
        TOKEN_TA_NOTIFIER = credentials('my_ta_notifier_api')
    }
    stages {
        stage('Parallel Builds') {
            failFast false
            parallel {
                stage('Documentation Pipeline') {
                    when {
                        branch 'main'
                    }
                    stages {
                        stage('Check Changes') {
                            steps {
                                script {
                                    changes = sh(
                                        script: "git diff-tree --no-commit-id --name-only -r HEAD | grep -E '^documentation/docusaurus/|^Jenkinsfile$' || true",
                                        returnStdout: true
                                    ).trim()
                                    if (!changes) {
                                        echo "No changes detected in /documentation/docusaurus or Jenkinsfile, ignoring Docker build."
                                    } else {
                                        echo "Changes detected in /documentation/docusaurus or Jenkinsfile:"
                                        echo changes
                                    }
                                }
                            }
                        }
                        stage('Docker Build and Publish') {
                            when {
                                expression { (!!changes) == true }
                            }
                            steps {
                                catchError(buildResult: 'UNSTABLE', stageResult: 'FAILURE') {
                                    script {
                                        sh '''
                                            cd documentation/docusaurus
                                            docker build -t rtype-documentation:latest .
                                        '''
                                        sh 'echo $GITHUB_GHCR_PAT | docker login ghcr.io -u a9ex --password-stdin'
                                        sh 'docker tag rtype-documentation:latest ghcr.io/epitechpromo2027/rtype-documentation:latest'
                                        sh 'docker push ghcr.io/epitechpromo2027/rtype-documentation:latest'
                                        sh 'curl --fail-with-body --header "Authorization: Bearer $TOKEN_TA_NOTIFIER" https://mytanotifier.a1ex.fr/api/build-rtype-docs'
                                    }
                                }
                            }
                            post {
                                always {
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
                                    image 'ghcr.io/a9ex/ubuntu-24-mingw:conan-deps'
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
                                    image 'ghcr.io/a9ex/ubuntu-24-mingw:conan-deps-win'
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
            }
        }
        stage('Create Release') {
            when {
                branch 'main'
            }
            steps {
                sh """
                    chmod +x ./create_github_release.sh
                    ./create_github_release.sh
                """
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
