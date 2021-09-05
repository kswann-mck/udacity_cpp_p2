FROM ubuntu:latest

WORKDIR /
RUN apt update -y
RUN apt install build-essential -y
RUN apt install libncurses5-dev -y
RUN apt install gcc -y
RUN apt install make -y
RUN DEBIAN_FRONTEND=noninteractive apt install cmake -y
RUN apt install htop -y
RUN apt install libncurses5-dev libncursesw5-dev -y

CMD ["/bin/bash"]

