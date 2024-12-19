#!/bin/bash

echo "RELEASE_TAG: START$RELEASE_TAG END"
if [ -z "$RELEASE_TAG" ]; then
    echo "Error: RELEASE_TAG environment variable is required"
    exit 1
fi

repo="444leod/R-Type"

echo "$GITHUB_TOKEN" | gh auth login --with-token

artifacts=$(find ./build -name "r-type_*" -type f)
if [ -z "$artifacts" ]; then
    echo "Error: No artifacts found matching r-type_*"
    exit 1
fi

echo "Uploading artifacts to release $RELEASE_TAG..."
for artifact in $artifacts; do
    echo "Uploading $artifact..."
    gh release upload "$RELEASE_TAG" "$artifact" --repo "$repo" --clobber
done

gh release edit "$RELEASE_TAG" --repo "$repo" --draft=false

gh auth logout

echo "Artifacts uploaded successfully!"
