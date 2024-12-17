def changes

pipeline {
    agent any
    environment {
        GITHUB_GHCR_PAT = credentials('github_pat_packages')
        TOKEN_TA_NOTIFIER = credentials('my_ta_notifier_api')
    }
    stages {
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
