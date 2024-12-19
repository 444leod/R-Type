#!/bin/bash

version=$(sed -n 's/project(rtype VERSION \([^)]*\))/\1/p' CMakeLists.txt)
if [ -z "$version" ]; then
    echo "Error: Could not extract version from CMakeLists.txt"
    exit 1
fi

increment_version() {
    local version=$1
    IFS='.' read -r major minor patch <<< "$version"
    patch=$((patch + 1))
    echo "$major.$minor.$patch"
}

tag="v${version}"
repo="444leod/R-Type"

while gh release view "$tag" --repo "$repo" &> /dev/null; do
    echo "Release $tag already exists. Incrementing version..."
    version=$(increment_version "$version")
    tag="v${version}"
done

echo "$GITHUB_TOKEN" | gh auth login --with-token

echo "Creating new release draft $tag..."
gh release create "$tag" \
    --repo "$repo" \
    --draft \
    --generate-notes \
    --title "R-Type ${version}"

gh auth logout

echo "$tag"
