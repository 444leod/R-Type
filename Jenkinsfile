def changes
def releaseTag
def commandOutput

pipeline {
    agent any
    environment {
        GITHUB_GHCR_PAT = credentials('github_pat_packages')
        TOKEN_TA_NOTIFIER = credentials('my_ta_notifier_api')
        GITHUB_TOKEN = credentials('github_pat_release')
    }
    stages {
        stage('Create GitHub Release Draft') {
            when {
                branch 'main'
            }
            agent {
                docker {
                    image 'ghcr.io/a9ex/ubuntu-24-mingw:conan-deps'
                    args '-u root'
                }
            }
            steps {
                script {
                    try {
                        commandOutput = sh(
                            script: """
                                chmod +x ./scripts/create_github_release.sh
                                ./scripts/create_github_release.sh
                            """,
                            returnStdout: true
                        )
                        releaseTag = commandOutput.split('\n').find { it.startsWith('TAG_START') }?.replaceAll('TAG_START(.*)TAG_END', '$1')

                        echo commandOutput
                        echo "Created release draft with tag: ${releaseTag}"
                    } catch (Exception e) {
                        echo "Failed to create release draft"
                        releaseTag = null
                        currentBuild.result = 'UNSTABLE'
                    }
                }
            }
        }
        stage('Parallel Builds') {
            failFast false
            parallel {
                stage('Format & Tidy') {
                    agent {
                        docker {
                            image 'ghcr.io/a9ex/ubuntu-24-mingw:conan-deps'
                            args '-u root'
                            alwaysPull true
                        }
                    }
                    stages {
                        stage('Setup') {
                            steps {
                                sh '''#!/bin/bash
                                    make clean
                                    make conan_ci
                                    source rtype_venv/bin/activate
                                    make deps
                                    cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
                                '''
                            }
                        }
                        stage('Format') {
                            steps {
                                catchError(buildResult: 'UNSTABLE', stageResult: 'FAILURE') {
                                    sh '''
                                        chmod +x ./scripts/check_format.sh
                                        ./scripts/check_format.sh
                                    '''
                                }
                            }
                        }
                        stage('Tidy') {
                            steps {
                                catchError(buildResult: 'UNSTABLE', stageResult: 'FAILURE') {
                                    sh '''
                                        chmod +x ./scripts/check_tidy.sh
                                        ./scripts/check_tidy.sh
                                    '''
                                }
                            }
                        }
                    }
                }
                stage('Documentation') {
                    when {
                        branch 'main'
                    }
                    stages {
                        stage('Check Changes') {
                            steps {
                                script {
                                    changes = sh(
                                        script: 'git diff-tree --no-commit-id --name-only -r HEAD | grep -E "^documentation/docusaurus/|^Jenkinsfile$" || true',
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
                        stage('Docker Build') {
                            when {
                                expression { (!!changes) == true }
                            }
                            steps {
                                script {
                                    env.DOCKER_BUILD_SUCCESS = 'false'
                                    catchError(buildResult: 'UNSTABLE', stageResult: 'FAILURE') {
                                        sh '''
                                            cd documentation/docusaurus
                                            docker build -t rtype-documentation:latest .
                                        '''
                                        env.DOCKER_BUILD_SUCCESS = 'true'
                                    }
                                }
                            }
                        }
                        stage('Docker Publish') {
                            when {
                                expression { (!!changes) == true && env.DOCKER_BUILD_SUCCESS == 'true' }
                            }
                            steps {
                                catchError(buildResult: 'UNSTABLE', stageResult: 'FAILURE') {
                                    script {
                                        sh 'echo $GITHUB_GHCR_PAT | docker login ghcr.io -u a9ex --password-stdin'
                                        sh 'docker tag rtype-documentation:latest ghcr.io/epitechpromo2027/rtype-documentation:latest'
                                        sh 'docker push ghcr.io/epitechpromo2027/rtype-documentation:latest'
                                    }
                                }
                            }
                        }
                        stage('Notify Server') {
                            when {
                                expression { (!!changes) == true && env.DOCKER_BUILD_SUCCESS == 'true' }
                            }
                            steps {
                                catchError(buildResult: 'UNSTABLE', stageResult: 'FAILURE') {
                                    script {
                                        sh 'curl --fail-with-body --header "Authorization: Bearer $TOKEN_TA_NOTIFIER" https://mytanotifier.a1ex.fr/api/build-rtype-docs'
                                    }
                                }
                            }
                        }
                        stage('Docker Cleanup') {
                            when {
                                expression { (!!changes) == true }
                            }
                            steps {
                                script {
                                    sh 'docker logout ghcr.io'
                                }
                            }
                        }
                    }
                }
                stage('Linux Build') {
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
                        stage('Prepare Linux Release') {
                            steps {
                                script {
                                    sh """
                                        chmod +x ./scripts/prepare_release_artifacts.sh
                                        ./scripts/prepare_release_artifacts.sh linux
                                    """
                                }
                            }
                        }
                        stage('Upload Linux Artifacts') {
                            steps {
                                script {
                                    if (env.BRANCH_NAME == 'main' && releaseTag != null) {
                                        sh """
                                            chmod +x ./scripts/upload_artifacts.sh
                                            RELEASE_TAG='${releaseTag}' ./scripts/upload_artifacts.sh
                                        """
                                    }
                                    archiveArtifacts artifacts: 'r-type_*.tar.gz', fingerprint: true
                                }
                            }
                        }
                    }
                    post {
                        always {
                            sh 'rm -rf ./* ./.git*'
                        }
                    }
                }
                stage('Windows Build') {
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
                        stage('Prepare Windows Release') {
                            steps {
                                script {
                                    sh """
                                        chmod +x ./scripts/prepare_release_artifacts.sh
                                        ./scripts/prepare_release_artifacts.sh windows
                                    """
                                }
                            }
                        }
                        stage('Upload Windows Artifacts') {
                            steps {
                                script {
                                    if (env.BRANCH_NAME == 'main' && releaseTag != null) {
                                        sh """
                                            chmod +x ./scripts/upload_artifacts.sh
                                            RELEASE_TAG='${releaseTag}' ./scripts/upload_artifacts.sh
                                        """
                                    }
                                    archiveArtifacts artifacts: 'r-type_*.tar.gz', fingerprint: true
                                }
                            }
                        }
                    }
                    post {
                        always {
                            sh 'rm -rf ./* ./.git*'
                        }
                    }
                }
            }
        }
    }
    post {
        always {
            sh 'sudo rm -rf ./* ./.git*'
            echo "Pipeline OK"
        }
        failure {
            echo "Pipeline KO"
        }
    }
}
