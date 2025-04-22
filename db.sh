#!/bin/bash

# Load environment variables from .env file if it exists
if [ -f .env ]; then
  export $(grep -v '^#' .env | xargs)
fi

# Default configuration (use values from environment variables or fallback to defaults)
CONTAINER_NAME="${DB_CONTAINER_NAME:-bankingsystem-db-1}"  # Replace with the default container name if not set
DB_USER="${MYSQL_USER:-your_db_user}"                      # Use MYSQL_USER from environment variables
DB_PASSWORD="${MYSQL_PASSWORD:-your_db_password}"          # Use MYSQL_PASSWORD from environment variables
DB_NAME="${MYSQL_DATABASE:-bank_db}"                       # Use MYSQL_DATABASE from environment variables

# Check if all required variables are set
if [ -z "$CONTAINER_NAME" ] || [ -z "$DB_USER" ] || [ -z "$DB_PASSWORD" ] || [ -z "$DB_NAME" ]; then
  echo "Error: Missing required environment variables."
  echo "Please ensure CONTAINER_NAME, DB_USER, DB_PASSWORD, and DB_NAME are set."
  exit 1
fi

# Connect to the database
echo "Connecting to database '${DB_NAME}' in container '${CONTAINER_NAME}'..."
docker exec -it "${CONTAINER_NAME}" mariadb -u"${DB_USER}" -p"${DB_PASSWORD}" "${DB_NAME}"

# Check for errors
if [ $? -ne 0 ]; then
  echo "Failed to connect to the database. Please check the container name, credentials, and database name."
  exit 1
else
  echo "Connected successfully!"
fi