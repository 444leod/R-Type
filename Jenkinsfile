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
                            sh 'docker logout'
                        }
                    }
                }
            }
        }
        stage('Build binaries') {
            stages {
                stage('Install deps') {
                    agent {
                        docker {
                            image 'ghcr.io/a9ex/epitech-devcontainer@sha256:3222291beff662c9570eff60887c0d8e0cf02e4e26f8f4f58f91cd7120095fa4'
                            args '-u root'
                            reuseNode true
                        }
                    }
                    steps {
                        script {
                            sh '''#!/bin/bash
                                make conan_ci
                                source rtype_venv/bin/activate
                                make deps
                                make
                            '''
                        }
                    }
                }
                stage('Build binaries') {
                    agent {
                        docker {
                            image 'ghcr.io/a9ex/epitech-devcontainer@sha256:3222291beff662c9570eff60887c0d8e0cf02e4e26f8f4f58f91cd7120095fa4'
                            args '-u root'
                            reuseNode true
                        }
                    }
                    steps {
                        script {
                            sh '''#!/bin/bash
                                echo test
                            '''
                        }
                    }
                }
                stage('Archive artifacts') {
                    steps {
                        archiveArtifacts artifacts: 'build/r-type*', fingerprint: true
                    }
                }
            }
        }
    }
    post {
        always {
            echo "Pipeline OK"
        }
        failure {
            echo "Pipeline KO"
        }
    }
}
