<?hh

class A {
    <<__LSB>> public static string $x;
}

class B extends A {
    public static string $x;
}

<<__EntryPoint>>
function main(): void {
  echo "Done.\n";
}
