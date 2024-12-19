#!/bin/bash

version=$(sed -n 's/project(rtype VERSION \([^)]*\))/\1/p' CMakeLists.txt)
if [ -z "$version" ]; then
    echo "Error: Could not extract version from CMakeLists.txt" >&2
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
    echo "Release $tag already exists. Incrementing version..." >&2
    version=$(increment_version "$version")
    tag="v${version}"
done

# Redirect gh command output to stderr
echo "$GITHUB_TOKEN" | gh auth login --with-token 1>&2

echo "Creating new release draft $tag..." >&2
gh release create "$tag" \
    --repo "$repo" \
    --draft \
    --generate-notes \
    --title "R-Type ${version}" 1>&2

gh auth logout 1>&2

# Only output that should be captured by Jenkins
echo -n "$tag"
