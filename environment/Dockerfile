FROM ubuntu:noble
ENV LANG=C.UTF-8 LC_ALL=C.UTF-8
ENV PATH=/opt/conda/bin:$PATH

# renovate: datasource=custom.anaconda_installer
ARG INSTALLER_URL_LINUX64="https://repo.anaconda.com/archive/Anaconda3-2024.06-1-Linux-x86_64.sh"
ARG SHA256SUM_LINUX64="539bb43d9a52d758d0fdfa1b1b049920ec6f8c6d15ee9fe4a423355fe551a8f7"
# renovate: datasource=custom.anaconda_installer
ARG INSTALLER_URL_S390X="https://repo.anaconda.com/archive/Anaconda3-2024.06-1-Linux-s390x.sh"
ARG SHA256SUM_S390X="1b3f4ef4147137fb1861f8251679ea96e651a7c405993c61e39441e43ee2dd11"
# renovate: datasource=custom.anaconda_installer
ARG INSTALLER_URL_AARCH64="https://repo.anaconda.com/archive/Anaconda3-2024.06-1-Linux-aarch64.sh"
ARG SHA256SUM_AARCH64="b4be0ad2052236882402902a31d32cd37635d3db194a42f977be0d68a8ff1a31"

# hadolint ignore=DL3008
RUN set -x && \
    apt-get update --fix-missing && \
    apt-get install -y --no-install-recommends \
        bzip2 \
        ca-certificates \
        git \
        libglib2.0-0 \
        libsm6 \
        libxcomposite1 \
        libxcursor1 \
        libxdamage1 \
        libxext6 \
        libxfixes3 \
        libxi6 \
        libxinerama1 \
        libxrandr2 \
        libxrender1 \
        mercurial \
        openssh-client \
        procps \
        subversion \
        wget \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* && \
    UNAME_M="$(uname -m)" && \
    if [ "${UNAME_M}" = "x86_64" ]; then \
        INSTALLER_URL=${INSTALLER_URL_LINUX64}; \
        SHA256SUM=${SHA256SUM_LINUX64}; \
    elif [ "${UNAME_M}" = "s390x" ]; then \
        INSTALLER_URL=${INSTALLER_URL_S390X}; \
        SHA256SUM=${SHA256SUM_S390X}; \
    elif [ "${UNAME_M}" = "aarch64" ]; then \
        INSTALLER_URL=${INSTALLER_URL_AARCH64}; \
        SHA256SUM=${SHA256SUM_AARCH64}; \
    fi && \
    wget "${INSTALLER_URL}" -O anaconda.sh -q && \
    echo "${SHA256SUM} anaconda.sh" > shasum && \
    sha256sum --check --status shasum && \
    /bin/bash anaconda.sh -b -p /opt/conda && \
    rm anaconda.sh shasum && \
    ln -s /opt/conda/etc/profile.d/conda.sh /etc/profile.d/conda.sh && \
    echo ". /opt/conda/etc/profile.d/conda.sh" >> ~/.bashrc && \
    echo "conda activate base" >> ~/.bashrc && \
    find /opt/conda/ -follow -type f -name '*.a' -delete && \
    find /opt/conda/ -follow -type f -name '*.js.map' -delete && \
    /opt/conda/bin/conda clean -afy


RUN apt-get update && apt-get install -y \
    build-essential \
    vim \
    zip

RUN conda install numba scikit-learn scikit-image jupyterlab pip matplotlib

RUN conda install -c conda-forge openjdk && \
    wget https://github.com/SpencerPark/IJava/releases/download/v1.3.0/ijava-1.3.0.zip && \ 
    unzip ijava-1.3.0.zip && \
    python install.py --sys-prefix && \
    rm install.py ijava-1.3.0.zip 

# Would prefer to install with conda, but doesn't work now
RUN pip install pyspark ray

#RUN conda install torchvision   
#RUN conda install pandoc   

#RUN conda install jax -c conda-forge

RUN wget https://dlcdn.apache.org/hadoop/common/hadoop-3.2.4/hadoop-3.2.4.tar.gz && \
        tar -xvzf hadoop-3.2.4.tar.gz && rm -f hadoop-3.2.4.tar.gz

ENV JAVA_HOME=/opt/conda

#RUN apt-get -y install graphviz
#RUN conda install python-graphviz

#RUN pip install -U ipywidgets

#RUN apt-get update && apt-get install -y \
#    openmpi-bin openmpi-common libopenmpi-dev

RUN useradd -ms /bin/bash jupyteruser
USER jupyteruser
WORKDIR /home/jupyteruser
ENV SHELL=/bin/bash

CMD [ "/bin/bash" ]
