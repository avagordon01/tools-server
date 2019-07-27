with import <nixpkgs> {};

stdenv.mkDerivation {
  name = "tools-server";
  src = ./.;

  buildInputs = [ libuv libwebsockets nlohmann_json openssl ];

  dontStrip = true;
  enableDebugging = true;

  installPhase = ''
    mkdir -p $out/bin
    cp harness muxer $out/bin/
  '';
}
