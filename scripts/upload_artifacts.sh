#!/bin/bash

if [ -z "$RELEASE_TAG" ]; then
    echo "Error: RELEASE_TAG environment variable is required"
    exit 1
fi

repo="444leod/R-Type"

echo "$GITHUB_TOKEN" | gh auth login --with-token

for tarball in r-type_*.tar.gz; do
    if [ -f "$tarball" ]; then
        echo "Uploading $tarball..."
        gh release upload "$RELEASE_TAG" "$tarball" --repo "$repo" --clobber
    fi
done

gh release edit "$RELEASE_TAG" --repo "$repo" --draft=false

gh auth logout

echo "Artifacts uploaded successfully!"
