{
  description = "bb";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    treefmt-nix.url = "github:numtide/treefmt-nix";
    nox.url = "github:playfairs/nox";
  };

  outputs =
    {
      self,
      nixpkgs,
      nox,
      treefmt-nix,
    }:
    let
      systems = [
        "x86_64-linux"
        "aarch64-linux"
        "x86_64-darwin"
        "aarch64-darwin"
      ];
      forAllSystems = nixpkgs.lib.genAttrs systems;
    in
    {
      formatter = forAllSystems (
        system:
        let
          pkgs = import nixpkgs { inherit system; };
        in
        import ./nix/formatter.nix { inherit pkgs treefmt-nix; }
      );

      devShells = forAllSystems (
        system:
        let
          pkgs = import nixpkgs { inherit system; };
          formatter = import ./nix/formatter.nix { inherit pkgs treefmt-nix; };
        in
        {
          default = pkgs.mkShell {
            buildInputs = with pkgs; [
              clang
              nox.packages.${system}.default
              formatter
            ];
          };
        }
      );
    };
}
