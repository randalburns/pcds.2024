FROM mcr.microsoft.com/devcontainers/miniconda
ENV LANG=C.UTF-8 LC_ALL=C.UTF-8

RUN conda install -c conda-forge \
        python=3.11 \
        numba \
        scikit-learn \
        scikit-image \
        jupyterlab \
        matplotlib \
        ijava \
        openjdk -y && \
    git config --global --add safe.directory .
