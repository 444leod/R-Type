#!/bin/bash

platform=$1
if [ -z "$platform" ]; then
    echo "Error: Platform (linux/windows) must be specified"
    exit 1
fi

# Create temporary directory for the release
release_dir="release_${platform}"
rm -rf "$release_dir"
mkdir -p "$release_dir"

# Copy binaries and assets
if [ "$platform" = "linux" ]; then
    cp build/client/r-type_client_linux "$release_dir/r-type_client"
    cp build/server/r-type_server_linux "$release_dir/r-type_server"
    tar_name="r-type_linux.tar.gz"
else
    cp build/client/r-type_client_windows.exe "$release_dir/r-type_client.exe"
    cp build/server/r-type_server_windows.exe "$release_dir/r-type_server.exe"
    tar_name="r-type_windows.tar.gz"
fi

# Copy assets directory
cp -r assets "$release_dir/"

# Create tarball
tar -czf "$tar_name" -C "$release_dir" .

# Cleanup
rm -rf "$release_dir"

echo "Created release archive: $tar_name" 
