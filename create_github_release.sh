#!/bin/bash

# Extract version from CMakeLists.txt
version=$(sed -n 's/project(rtype VERSION \([^)]*\))/\1/p' CMakeLists.txt)
if [ -z "$version" ]; then
    echo "Error: Could not extract version from CMakeLists.txt"
    exit 1
fi

# Set tag based on version
tag="v${version}"
repo="444leod/R-Type"

RELEASE_ID=$(curl -s -H "Authorization: token $GITHUB_TOKEN" \
                                "https://api.github.com/repos/$repo/releases/tags/$tag" | \
                                grep -o '"id": [0-9]\\+' | head -n1 | cut -d' ' -f2 || echo '')

if [ ! -z "$RELEASE_ID" ]; then
    curl -X DELETE -H "Authorization: token $GITHUB_TOKEN" \
        "https://api.github.com/repos/$repo/releases/$RELEASE_ID"
    curl -X DELETE -H "Authorization: token $GITHUB_TOKEN" \
        "https://api.github.com/repos/$repo/git/refs/tags/$tag"
fi

RELEASE_RESPONSE=$(curl -X POST \
    -H "Authorization: token $GITHUB_TOKEN" \
    -H "Content-Type: application/json" \
    "https://api.github.com/repos/$repo/releases" \
    -d '{
        "tag_name": "'$tag'",
        "name": "R-Type '${version}'",
        "body": "Release '${version}'",
        "draft": false,
        "prerelease": false
    }')

UPLOAD_URL=$(echo "$RELEASE_RESPONSE" | grep -o '"upload_url": "[^"]*' | cut -d'"' -f4 | sed 's/{?name,label}//')

for artifact in $(find ./build -name "r-type_*" -type f); do
    filename=$(basename "$artifact")
    curl -X POST \
        -H "Authorization: token $GITHUB_TOKEN" \
        -H "Content-Type: application/octet-stream" \
        "$UPLOAD_URL?name=$filename" \
        --data-binary @"$artifact"
done
