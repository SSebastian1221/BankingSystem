#!/bin/bash

# Load environment variables from .env file if it exists
if [ -f .env ]; then
  export $(grep -v '^#' .env | xargs)
fi

# Default configuration (use values from environment variables or fallback to defaults)
CONTAINER_NAME="${DB_CONTAINER_NAME:-bankingsystem-db-1}"  # Replace with the default container name if not set

# Check if all required variables are set
if [ -z "$CONTAINER_NAME" ] || [ -z "$MYSQL_USER" ] || [ -z "$MYSQL_PASSWORD" ] || [ -z "$MYSQL_DATABASE" ]; then
  echo "Error: Missing required environment variables."
  echo "Please ensure CONTAINER_NAME, DB_USER, DB_PASSWORD, and DB_NAME are set."
  exit 1
fi

# Connect to the database
echo "Connecting to database '${DB_NAME}' in container '${CONTAINER_NAME}'..."
docker exec -it "${CONTAINER_NAME}" mariadb -u"${MYSQL_USER}" -p"${MYSQL_PASSWORD}" "${MYSQL_DATABASE}"

# Check for errors
if [ $? -ne 0 ]; then
  echo "Failed to connect to the database. Please check the container name, credentials, and database name."
  exit 1
else
  echo "Connected successfully!"
fi
