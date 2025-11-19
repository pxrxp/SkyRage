IF NOT EXIST ".\vcpkg" (
    git clone https://github.com/microsoft/vcpkg.git vcpkg
    call .\vcpkg\bootstrap-vcpkg.bat
)
