<?php
/* Prototype  : bool usort(&array $array_arg, string $cmp_function)
 * Description: Sort an array by values using a user-defined comparison function 
 * Source code: ext/standard/array.c
 */

/*
 * Pass an anonymous comparison function as $cmp_function argument to test behaviour()
 */

echo "*** Testing usort() : usage variation ***\n";

$cmp_function = ($value1, $value2) ==> { if($value1 == $value2) {return 0;} else if($value1 > $value2) {return 1;} else{return -1;} };
$cmp_function_ref = (&$value1, &$value2) ==> { if($value1 == $value2) {return 0;} else if($value1 > $value2) {return 1;} else{return -1;} };

$array_arg = array(0 => 100, 1 => 3, 2 => -70, 3 => 24, 4 => 90);

echo "\n-- Anonymous 'cmp_function' with parameters passed by value --\n";
var_dump( usort(&$array_arg, $cmp_function ) );
var_dump($array_arg);

$array_arg = array("b" => "Banana", "m" => "Mango", "a" => "Apple", "p" => "Pineapple");

echo "\n-- Anonymous 'cmp_function' with parameters passed by reference --\n";
var_dump( usort(&$array_arg, $cmp_function_ref) );
var_dump($array_arg);
?>
===DONE===
