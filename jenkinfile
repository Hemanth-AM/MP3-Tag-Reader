pipeline {
    agent any

    stages {

        stage('Build') {
            steps {
                dir('MP3_Tag_Reader_skeleton code - Copy (3)') {
                    bat 'gcc *.c -o app.exe'
                }
            }
        }

        stage('Run') {
            steps {
                dir('MP3_Tag_Reader_skeleton code - Copy (3)') {
                    bat 'app.exe'
                }
            }
        }
    }
}
