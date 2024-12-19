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

artifacts=$(find ./build -name "r-type_*" -type f)
if [ -z "$artifacts" ]; then
    echo "Error: No artifacts found matching r-type_*"
    exit 1
fi

echo "Creating new release $tag..."
gh release create "$tag" \
    --repo "$repo" \
    --generate-notes \
    --title "R-Type ${version}" \
    $artifacts

gh auth logout

echo "Release $tag created successfully!"
