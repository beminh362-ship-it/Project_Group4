FROM gcc:latest
WORKDIR /usr/src/app
COPY . .
RUN g++ -o myapp main.cpp AttendanceSession.cpp Class.cpp CSVHelper.cpp Lecturer.cpp Member.cpp Student.cpp Utils.cpp
CMD ["./myapp"]