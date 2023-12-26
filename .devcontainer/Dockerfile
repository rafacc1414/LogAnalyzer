FROM ubuntu:latest

ENV DEV_PATH=/code/dev

RUN apt-get update
# RUN apt-get install -y build-essential && \
#     apt-get install -y wget
RUN apt-get install -y python3 && \
    apt-get install -y git && \
    apt-get install -y python3-pip && \
    apt-get install -y vim && \
    apt-get install -y libpq-dev

# Cmake Installation
ARG OS

RUN apt-get update && apt-get install -y build-essential wget
# Install CMake based on the specified OS
RUN if [ "$OS" = "aarch64" ]; then \
        wget https://github.com/Kitware/CMake/releases/download/v3.28.1/cmake-3.28.1-linux-aarch64.sh -q -O /tmp/cmake-install.sh; \
    elif [ "$OS" = "x86_64" ]; then \
        wget https://github.com/Kitware/CMake/releases/download/v3.28.1/cmake-3.28.1-linux-x86_64.sh -q -O /tmp/cmake-install.sh; \
    else \
        echo "No valid OS specified"; \
    fi && \
    chmod u+x /tmp/cmake-install.sh && \
    mkdir -p /opt/cmake-3.28.1 && \
    /tmp/cmake-install.sh --skip-license --prefix=/opt/cmake-3.28.1 && \
    rm /tmp/cmake-install.sh && \
    ln -s /opt/cmake-3.28.1/bin/* /usr/local/bin

RUN pip3 install conan && \
    conan profile detect --force

WORKDIR $DEV_PATH

RUN useradd -ms /bin/bash dev
RUN chown -R dev:dev $DEV_PATH
USER dev

CMD ["/bin/bash"]