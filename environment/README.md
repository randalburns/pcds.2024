### Building the Parallel Computing EBook environment

These are instructions as to how to create a dockerized environment for the parallel programming eBook.

#### Prepare your system.

Install docker.  Follow platform specific instructions

#### Build container

docker build . -t pp.ebook

#### Run container

docker run -i -t --mount type=bind,source="/$(pwd)"/../ebook,target=/home/jupyteruser/ebook -p8888:8888 ppenv


### Run jupyter in container

jupyter lab --ip 0.0.0.0 --allow-root


#### Notes for randal

# add to dockerfile
conda install -c conda-forge xeus-cling

# for ubuntu?
conda install -c conda-forge openmp

# for mac
conda install -c conda-forge llvm-openmp

# RB remove allow root later
jupyter lab --ip 0.0.0.0 --allow-root
#docker run -i -t --mount type=bind,source="/$(pwd)"/../pplectures2022,target=/pplectures2022 -p8888:8888 ppenv


