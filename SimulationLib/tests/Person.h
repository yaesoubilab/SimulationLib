#pragma once

enum class FavoriteColor {
    Blue, Red, Green
};

using NumberDogs = unsigned int;

using Person = struct {
    FavoriteColor  color;
    NumberDogs     nDogs;
};

Person newPerson(int _nDogs, FavoriteColor _color) {
    Person per;

    per.nDogs  = _nDogs;
    per.color  = _color;

    return per;
}

Person changeFavoriteColor(Person per, FavoriteColor _color) {
    per.color = _color;
    return per;
}

Person incrementDogs(Person per, int _nDogs) {
    per.nDogs += _nDogs;
    return per;
}
