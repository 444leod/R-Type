pipeline {
    agent any
    triggers {
        githubPush()
    }
    environment {
        GITHUB_GHCR_PAT = credentials('github_pat_packages')
        DOCS_CHANGED = 'false'
    }
    stages {
        stage('Build, Publish, Deploy Docusaurus') {
            stages {
                stage('Check Changes') {
                    steps {
                        script {
                            def changes = sh(
                                script: "git diff-tree --no-commit-id --name-only -r HEAD | grep '^documentation/docusaurus/' || true",
                                returnStdout: true
                            ).trim()
                            if (!changes) {
                                echo "No changes detected in /documentation/docusaurus, ignoring Docker build."
                            } else {
                                echo "Changes detected in /documentation/docusaurus :"
                                echo changes
                                env.DOCS_CHANGED = 'true'
                            }
                        }
                    }
                }
                stage('Docker Build and Publish') {
                    when {
                        expression { env.DOCS_CHANGED == 'true' }
                    }
                    stages {
                        stage('Build Docker Image') {
                            steps {
                                script {
                                    sh 'docker build -t rtype-documentation:latest -f documentation/docusaurus/Dockerfile .'
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
            }
        }
        stage('Other') {
            steps {
                echo "OK"
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
