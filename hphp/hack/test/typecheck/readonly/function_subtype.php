<?hh
<<file:__EnableUnstableFeatures('readonly')>>
class ParentClass {
  public int $prop = 2;

  public function set(int $y) : void {
    $this->prop = $y;
  }

  public readonly function get() : ParentClass {
    return new ParentClass();
  }

  public function return_ro(): ParentClass {
    return new ParentClass();
  }

  public function constFun(
    (function (Child) : Child) $f
  ) : void {
  }

  public function setParams(readonly ParentClass $x, ParentClass $y) : void {
  }
}


class Child extends ParentClass {
  public readonly function set(int $y) : void {} // ok

  public function get() : readonly ParentClass {
      $this->prop = 6;
      return readonly new ParentClass();
  }

  public function return_ro(): readonly ParentClass {
    return readonly new ParentClass();
  }

  public function setParams(
    ParentClass $x, // Cannot override readonly parameter with mutable
    readonly ParentClass $y // ok
  ) : void {
  }

  public function constFun(
    <<__ConstFun>> (function (Child) : Child) $f
  ) : void {
  }


}

function fnTypes(
  (function(readonly Child) : Child) $f,
  (function (readonly Child) : readonly Child) $f2,
) : void {
  $y = $f(new Child()); // error, argument is not readonly
  $z = function(Child $x) : Child {
    return $x;
  };
  // fails because the function takes mutable args which is not a subtype
  fnTypes($z, $f2);
  // fails because the function returns readonly Child instead of mutable Child
  fnTypes($f2, $f2);
}
