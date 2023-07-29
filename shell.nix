{ pkgs ? import <nixpkgs> {} }:

with pkgs;

mkShell {
  buildInputs = [
    pkg-config
    clang-tools
    nlohmann_json
    curl
    gdb
    harfbuzz.dev
    icu.dev
    lzo
  ] ++ openttd-jgrpp.buildInputs ++ openttd-jgrpp.nativeBuildInputs;
  shellHook = ''
    if test -f ./shell.nix; then
      mkdir -p ./build
      cd ./build
      test -L ./share && rm ./share
      ln -s ${openttd-jgrpp}/share ./share
      cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_INSTALL_PREFIX=./  ..
    else
      echo "Something went wrong probably!"
    fi
  '';

}
