# Encoding Integers


| C Data Type   | Size on 32-bit System | Size on 64-bit System | Signed Range (32-bit)                                | Unsigned Range (32-bit)            | Signed Range (64-bit)                                | Unsigned Range (64-bit)            |
|---------------|-----------------------|-----------------------|-----------------------------------------------------|------------------------------------|-----------------------------------------------------|------------------------------------|
| `char`        | 1                     | 1                     | -128 to 127                                          | 0 to 255                           | -128 to 127                                          | 0 to 255                           |
| `short`       | 2                     | 2                     | -32,768 to 32,767                                    | 0 to 65,535                        | -32,768 to 32,767                                    | 0 to 65,535                        |
| `int`         | 4                     | 4                     | -2,147,483,648 to 2,147,483,647                      | 0 to 4,294,967,295                 | -2,147,483,648 to 2,147,483,647                      | 0 to 4,294,967,295                 |
| `long`        | 4                     | 8                     | -2,147,483,648 to 2,147,483,647                      | 0 to 4,294,967,295                 | -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807 | 0 to 18,446,744,073,709,551,615    |
| `long long`   | 8                     | 8                     | -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807 | 0 to 18,446,744,073,709,551,615    | -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807 | 0 to 18,446,744,073,709,551,615    |
| `float`       | 4                     | 4                     | ~-3.4E+38 to ~3.4E+38                                | N/A                                | ~-3.4E+38 to ~3.4E+38                                | N/A                                |
| `double`      | 8                     | 8                     | ~-1.7E+308 to ~1.7E+308                              | N/A                                | ~-1.7E+308 to ~1.7E+308                              | N/A                                |
| `long double` | 12 or 16              | 16                    | ~-3.4E+4932 to ~1.1E+4932                            | N/A                                | ~-3.4E+4932 to ~1.1E+4932                            | N/A                                |
| `pointer`     | 4                     | 8                     | Platform dependent                                   | Platform dependent                 | Platform dependent                                   | Platform dependent                 |

This table now clearly separates the signed and unsigned ranges for each data type on both 32-bit and 64-bit systems.
