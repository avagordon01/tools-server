with import <nixpkgs> {};

stdenv.mkDerivation {
  name = "tools-server";
  src = ./.;

  buildInputs = [ libuv libwebsockets ];

  installPhase = ''
    mkdir -p $out/bin
    cp harness muxer $out/bin/
  '';
}
