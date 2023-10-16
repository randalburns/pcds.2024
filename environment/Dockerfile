FROM ubuntu:lunar
ENV LANG=C.UTF-8 LC_ALL=C.UTF-8
ENV PATH /opt/conda/bin:$PATH

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
        ANACONDA_URL="https://repo.anaconda.com/archive/Anaconda3-2023.03-1-Linux-x86_64.sh"; \
        SHA256SUM="95102d7c732411f1458a20bdf47e4c1b0b6c8a21a2edfe4052ca370aaae57bab"; \
    elif [ "${UNAME_M}" = "s390x" ]; then \
        ANACONDA_URL="https://repo.anaconda.com/archive/Anaconda3-2023.03-1-Linux-s390x.sh"; \
        SHA256SUM="5af1406c6350b4ba6839c49faa32a3c90f2b9845a03c35843f118dd9dd013421"; \
    elif [ "${UNAME_M}" = "aarch64" ]; then \
        ANACONDA_URL="https://repo.anaconda.com/archive/Anaconda3-2023.03-1-Linux-aarch64.sh"; \
        SHA256SUM="54e600faa2af63a25717af30ecaddf1ee428cdfebd3721a70f41462e232e8153"; \
    elif [ "${UNAME_M}" = "ppc64le" ]; then \
        ANACONDA_URL="https://repo.anaconda.com/archive/Anaconda3-2023.03-1-Linux-ppc64le.sh"; \
        SHA256SUM="a31f2d6da83534cff7c994403cc11fa634b31fcd10eb4153d00233345ee084b2"; \
    fi && \
    wget "${ANACONDA_URL}" -O anaconda.sh -q && \
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

RUN conda install torchvision   
RUN conda install pandoc   

RUN conda install jax -c conda-forge

RUN wget https://dlcdn.apache.org/hadoop/common/hadoop-3.2.4/hadoop-3.2.4.tar.gz && \
        tar -xvzf hadoop-3.2.4.tar.gz && rm -f hadoop-3.2.4.tar.gz

ENV JAVA_HOME=/opt/conda/bin
ENV HADOOP_CLASSPATH=${JAVA_HOME}/lib/tools.jar
    
RUN useradd -ms /bin/bash jupyteruser
USER jupyteruser
WORKDIR /home/jupyteruser

CMD [ "/bin/bash" ]
