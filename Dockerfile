# Install all packajes and build ------------------------------------------------------------------------

# Install gcc latest version
FROM gcc:11

# Use ubuntu:latest as the base image
FROM ubuntu:22.04


# Install python3, cmake
RUN apt-get update && \
    apt-get install -y \
    cmake \
    python3 \
    python3-pip

# Install conan 2
RUN pip install conan && \
    conan profile detect --force

# Copy the /server directory to the container
ADD . /app/http_server

# Set the working directory for building the project
WORKDIR /app/http_server/build

# Build our project and test it
RUN cmake .. -DDEBUG=ON
RUN cmake --build . -j $(nproc)

# Run --------------------------------------------------------------------------------------------------

# Set the entry point
ENTRYPOINT ["/app/http_server/build/http_server"]