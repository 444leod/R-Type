#!/bin/bash

repo="444leod/R-Type"
platform=$1

if [ -z "$platform" ]; then
    echo "Error: Platform (linux/windows) must be specified"
    exit 1
fi

echo "$GITHUB_TOKEN" | gh auth login --with-token

git config --global --add safe.directory '*'

commit_sha=$(git rev-parse HEAD)

temp_dir="temp_comment_${commit_sha}"
mkdir -p "$temp_dir"

cp r-type_*.tar.gz "$temp_dir/" 2>/dev/null || true

gh api \@
  --method POST \
  "/repos/${repo}/commits/${commit_sha}/comments" \
  -F "body=🔨 ${platform^} binaries for commit ${commit_sha:0:7} successfully built.

These binaries were automatically built from this commit." \
  $(for file in "$temp_dir"/*; do echo -F "attachments[]=@$file"; done)

rm -rf "$temp_dir"
gh auth logout

echo "Artifacts commented successfully!"
