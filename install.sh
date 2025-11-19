if [ ! -d "./vcpkg" ]; then                                                         ─╯
    git clone https://github.com/microsoft/vcpkg.git vcpkg
    ./vcpkg/bootstrap-vcpkg.sh -disableMetrics
fi
