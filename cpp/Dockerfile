FROM ubuntu:22.04

ARG DEBIAN_FRONTEND=noninteractive
ARG UID=1000
ARG GID=1000

RUN apt-get -y update && \
    apt-get -y install \
    build-essential \
    clang-14 \
    clang-format-14 \
    clang-tidy-14 \
    cmake \
    gdb \
    vim \
    pkg-config \
    zlib1g-dev \
    libsfml-dev \
    sudo \
    # git \
    # doxygen \
    # this is for make completion
    bash-completion && \
    # change root password to 123
    echo 'root:123' | chpasswd && \
    # create a user foo and specify its uid and gid same as the host user
    groupadd -g ${GID} foo && \
    useradd -m -u ${UID} -g ${GID} -s /bin/bash foo && \
    echo 'foo:123' | chpasswd && \
    # although useradd -m will create a home directory, check its permission for safe
    mkdir -p /home/foo && \
    chown -R foo:foo /home/foo && \
    # add foo to sudo group
    usermod -aG sudo foo && \
    # this is for make completion
    echo 'source /etc/bash_completion' >> /home/foo/.bashrc && \
    echo 'source /usr/share/bash-completion/completions/make' >> /home/foo/.bashrc

USER foo

WORKDIR /home/foo/utilities/cpp

ENV CC=clang-14
ENV CXX=clang++-14

CMD ["tail", "-f", "/dev/null"]
