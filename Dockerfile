# ========================
# Stage 1: Build the app
# ========================
FROM ubuntu:22.04 AS builder

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libssl-dev \
    default-libmysqlclient-dev \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy source code and build files
COPY . .

# Create build directory and compile
RUN mkdir -p build && cd build && \
    cmake .. && \
    make

# ========================
# Stage 2: Runtime image
# ========================
FROM ubuntu:22.04

# Install runtime dependencies (the actual shared library + dev headers for linking)
RUN apt-get update && apt-get install -y \
    libmysqlclient21 \
    default-libmysqlclient-dev \
    libssl3 \
    && rm -rf /var/lib/apt/lists/*

# Copy the built binary from the builder stage
COPY --from=builder /app/build/bank_app /usr/local/bin/

# Set working directory
WORKDIR /usr/local/bin

# Run the application
CMD ["./bank_app"]

