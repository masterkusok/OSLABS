FROM ubuntu:latest

RUN apt-get update && apt-get install -y \
    build-essential \
    strace

COPY . /app

WORKDIR /app

RUN  g++ -std=c++17 main.cpp iolib/std.cpp

CMD ["/bin/bash"]
