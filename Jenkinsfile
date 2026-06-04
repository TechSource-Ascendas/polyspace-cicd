pipeline {
    agent any

    environment {
        RESULT_DIR = "ResultBF"
        PROG = "Bug_Finder_Example"
    }

    stages {
        stage('Build') {
            steps {
                sh 'ls -la src'
            }
        }

        stage('Build & Polyspace Configure') {
            steps {
                sh '''
                /usr/local/Polyspace_Server/R2025b/polyspace/bin/polyspace-configure \
                    -allow-overwrite \
                    -allow-build-error \
                    -prog $PROG \
                    -author jenkins \
                    -output-options-file $PROG.psopts \
                    gcc -I./src/include -c src/*.c
                '''
            }
        }

        stage('Run Polyspace') {
			steps {
				withCredentials([usernamePassword(credentialsId: 'demo user', usernameVariable: 'USER', passwordVariable: 'PASS')]) {
					sh '''
					/usr/local/Polyspace_Server/R2025b/polyspace/bin/polyspace-bug-finder-server \
						-options-file ${PROG}.psopts \
						-results-dir ${RESULT_DIR}

					/usr/local/Polyspace_Server/R2025b/polyspace/bin/polyspace-access \
						-host localhost \
						-port 9443 \
						-protocol http \
						-login $USER \
						-encrypted-password $PASS \
						-create-project /public/${PROG}

					/usr/local/Polyspace_Server/R2025b/polyspace/bin/polyspace-access \
						-host localhost \
						-port 9443 \
						-protocol http \
						-login $USER \
						-encrypted-password $PASS \
						-upload ${RESULT_DIR} \
						-project /public/${PROG}
					'''
				}
			}
		}
    }
}