# Use a base image with C++ build tools
FROM ubuntu:22.04 AS builder

# Install required packages
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libmysqlclient-dev \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy the source code and CMake files
COPY CMakeLists.txt .
COPY bank_app.cpp .

# Build the application
RUN mkdir build && cd build && \
    cmake .. && \
    make

# Create the final runtime image
FROM ubuntu:22.04

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
    libmysqlclient21 \
    && rm -rf /var/lib/apt/lists/*

# Copy the built executable from builder
COPY --from=builder /app/build/bank_app /usr/local/bin/

# Set working directory
WORKDIR /usr/local/bin

# Command to run the application
CMD ["./bank_app"]