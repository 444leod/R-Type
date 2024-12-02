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
            steps {
                stage('Check Changes') {
                    steps {
                        script {
                            def changes = sh(
                                script: "git diff-tree --no-commit-id --name-only -r HEAD | grep '^documentation/docusaurus/' || true",
                                returnStdout: true
                            ).trim()
                            if (!changes) {
                                echo "No changes detected in documentation, ignoring pipeline."
                            } else {
                                echo "Changes detected in /documentation/docusaurus :"
                                echo changes
                                env.DOCS_CHANGED = 'true'
                            }
                        }
                    }
                }
                stage('Docker') {
                    when {
                        expression { env.DOCS_CHANGED == 'true' }
                    }
                    steps {
                        stage('Build docker') {
                            steps {
                                script {
                                    sh 'docker build -t rtype-documentation:latest -f documentation/docusaurus/Dockerfile .'
                                }
                            }
                        }
                        stage('Push') {
                            steps {
                                sh 'echo $GITHUB_GHCR_PAT | docker login ghcr.io -u a9ex --password-stdin'
                                sh 'docker tag rtype-documentation:latest ghcr.io/epitechpromo2027/rtype-documentationi:latest'
                                sh 'docker push ghcr.io/epitechpromo2027/rtype-documentation:latest'
                            }
                        }
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
