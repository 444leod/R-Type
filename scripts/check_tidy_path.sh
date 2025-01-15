#!/bin/bash

build_dir="build"
check_path=$1

if [ -z "$check_path" ]; then
    echo "Error: Path to check must be specified"
    exit 1
fi

if [ "$check_path" = "lib" ]; then
    files=$(find . -type f \( -name "*.cpp" -o -name "*.hpp" \) \
        -not -path "./build/*" \
        -not -path "./client/*" \
        -not -path "./server/*")
else
    files=$(find "$check_path" -type f \( -name "*.cpp" -o -name "*.hpp" \) \
        -not -path "./build/*")
fi

if ! command -v clang-tidy-19 &> /dev/null; then
    clang_tidy_command="clang-tidy"
else
    clang_tidy_command="clang-tidy-19"
fi

tidy_issues=0
for file in $files; do
    echo "Checking $file..."
    if ! $clang_tidy_command \
        -p="${build_dir}" \
        --extra-arg="-I${build_dir}" \
        --warnings-as-errors=* \
        "$file"; then
        tidy_issues=1
    fi
done

if [ $tidy_issues -eq 1 ]; then
    echo "❌ Some files in $check_path have tidy issues."
    exit 1
else
    echo "✅ All files in $check_path pass tidy checks!"
fi
