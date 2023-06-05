#!/usr/bin/env bash
set -ex

# Creating flatpak directories
mkdir -p "${APPVEYOR_BUILD_FOLDER}/.flatpak/lib" "${APPVEYOR_BUILD_FOLDER}/.flatpak/data" "${APPVEYOR_BUILD_FOLDER}/.flatpak/bin"

cd "$APPVEYOR_BUILD_FOLDER/.build/build"

for config in Release Debug RelWithDebInfo
do
    cmake .. -DCMAKE_BUILD_TYPE=$config
    make -j$(nproc)
    make package
done

# cp -r "${APPVEYOR_BUILD_FOLDER}/data" "${APPVEYOR_BUILD_FOLDER}/.flatpak/"
# cd ${APPVEYOR_BUILD_FOLDER}

# # Editing metadatas with the current version
# export APPVEYOR_BUILD_DATE=$(date "+%Y-%m-%d")
# sed -i -e "s/V_VERSION/$APPVEYOR_BUILD_VERSION/g" -e "s/V_DATE/$APPVEYOR_BUILD_DATE/g" .flatpak/io.github.opendriver2.Redriver2.appdata.xml

# # Build the flatpak
# flatpak-builder --user --install build io.github.opendriver.redriver2.yaml --force-clean --arch=x86_64
# flatpak build-bundle ~/.local/share/flatpak/repo io.github.opendriver2.Redriver2.flatpak io.github.opendriver2.Redriver2
# rm -rf build/ .flatpak-builder/
