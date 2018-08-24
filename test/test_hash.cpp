#include <list>
#include <string>
#include <algorithm>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include "test.h"
#include "hash/hash_value.hpp"

USING_NAMESPACE_GOOFER

struct Person
{
    bool            gender;
    int             age;
    float           height;
    double          weight;
    std::string     first_name;
    std::string     last_name;
    std::string     province;

    Person()
        : gender(false)
        , age(0)
        , height(0.0f)
        , weight(0.0)
        , first_name()
        , last_name()
        , province()
    {

    }

    Person(bool g, int a, float h, double w, const std::string & f, const std::string & l, const std::string & p)
        : gender(g)
        , age(a)
        , height(h)
        , weight(w)
        , first_name(f)
        , last_name(l)
        , province(p)
    {

    }

    Person(const Person & other)
        : gender(other.gender)
        , age(other.age)
        , height(other.height)
        , weight(other.weight)
        , first_name(other.first_name)
        , last_name(other.last_name)
        , province(other.province)
    {

    }

    bool operator == (const Person & other) const
    {
        return (
            gender == other.gender && age == other.age && height == other.height && weight == other.weight && 
            first_name == other.first_name && last_name == other.last_name && province == other.province
        );
    }
};

std::ostream & operator << (std::ostream & os, const Person & person)
{
    return (os << "[" << person.gender << "," << person.age << "," << person.height << "," << person.weight << "," 
               << person.first_name << "," << person.last_name << "," << person.province << "]");
}

namespace std
{
    template <>
    struct hash<Person>
    {
        std::size_t operator() (const Person & person) const
        {
            std::size_t seed = 0;
            hash_combine(seed, person.gender);
            hash_combine(seed, person.age);
            hash_combine(seed, person.height);
            hash_combine(seed, person.weight);
            hash_combine(seed, person.first_name);
            hash_combine(seed, person.last_name);
            hash_combine(seed, person.province);
            return (seed);
        }
    };
}

struct People
{
    std::string         name;
    std::list<Person>   person_list;

    bool operator == (const People & other) const
    {
        return (name == other.name && person_list == other.person_list);
    }
};

std::ostream & operator << (std::ostream & os, const People & people)
{
    os << "{";
    os << people.name << std::endl;
    std::for_each(people.person_list.begin(), people.person_list.end(), [&os](const Person & person) {os << person << std::endl; });
    os << "}";
    return (os);
}

namespace std
{
    template <>
    struct hash<People>
    {
        std::size_t operator() (const People & people) const
        {
            std::size_t seed = 0;
            hash_combine(seed, people.name);
            hash_combine(seed, hash_range(people.person_list.begin(), people.person_list.end()));
            return (seed);
        }
    };
}

#define SHOW_HASH_VALUE(T, t)       \
std::cout << "hash<" << #T << ">(" << t << "): [" << hash_value(t) << "]" << std::endl;

#define SHOW_HASH_ARRAY(T, N, t)    \
std::cout << "hash<" << #T << "[" << #N << "]" << ">(" << t << "): [" << hash_value(t) << "]" << std::endl;

void test_hash(void)
{
    SHOW_HASH_VALUE(bool, false);
    SHOW_HASH_VALUE(bool, true);
    SHOW_HASH_VALUE(short, 0);
    SHOW_HASH_VALUE(short, 1);
    SHOW_HASH_VALUE(int, 123);
    SHOW_HASH_VALUE(int, 124);
    SHOW_HASH_VALUE(int, -123);
    SHOW_HASH_VALUE(int, -124);
    SHOW_HASH_VALUE(float, 123.0);
    SHOW_HASH_VALUE(float, 124.0);
    SHOW_HASH_VALUE(float, -123.0);
    SHOW_HASH_VALUE(float, -124.0);
    SHOW_HASH_VALUE(double, 123.0);
    SHOW_HASH_VALUE(double, 124.0);
    SHOW_HASH_VALUE(double, -123.0);
    SHOW_HASH_VALUE(double, -124.0);
    SHOW_HASH_VALUE(char *, "hello world");
    const char a[] = "123";
    const char b[] = "123";
    SHOW_HASH_VALUE(char *, a);
    SHOW_HASH_VALUE(char *, b);
    SHOW_HASH_VALUE(std::string, "hello world");
    SHOW_HASH_VALUE(std::string, "hello world");

    Person person{ false, 18, 154.5f, 45.5, "nana", "chen", "sz" };
    SHOW_HASH_VALUE(Person, person);
    People people;
    people.name = "family";
    people.person_list.emplace_back(Person{ false, 18, 154.5f, 45.5, "nana", "chen", "sz" });
    SHOW_HASH_VALUE(People, people);
    people.person_list.emplace_back(Person{ true, 25, 165.0f, 55.5, "vivi", "yang", "cd" });
    SHOW_HASH_VALUE(People, people);
    people.person_list.emplace_back(Person{ true, 31, 175.0f, 65.5, "jojo", "chou", "jx" });
    SHOW_HASH_VALUE(People, people);

    {
        std::cout << "1" << std::endl;
        Person per_vec[]{ person };
        SHOW_HASH_ARRAY(Person, 1, per_vec);
    }
    {
        std::cout << "2" << std::endl;
        Person per_vec[]{ person, person };
        SHOW_HASH_ARRAY(Person, 2, per_vec);
    }
    {
        std::cout << "3" << std::endl;
        Person per_vec[]{ person, person, person };
        SHOW_HASH_ARRAY(Person, 3, per_vec);
    }
    {
        std::cout << "4" << std::endl;
        Person per_vec[]{ person, person, person, person };
        SHOW_HASH_ARRAY(Person, 4, per_vec);
    }
    {
        std::cout << "5" << std::endl;
        Person per_vec[]{ person, person, person, person, person };
        SHOW_HASH_ARRAY(Person, 5, per_vec);
    }

    {
        Person person{ false, 18, 154.5f, 45.5, "nana", "chen", "sz" };
        std::unordered_set<Person, std::hash<Person>> set;
        std::unordered_map<Person, int, std::hash<Person>> map;
        map[person] = 1;
        set.insert(person);
        person.first_name = "coco";
        map[person] = 2;
        set.insert(person);
    }

    {
        Person person{ false, 18, 154.5f, 45.5, "nana", "chen", "sz" };
        std::unordered_set<Person> set;
        std::unordered_map<Person, int> map;
        map[person] = 1;
        set.insert(person);
        person.first_name = "coco";
        map[person] = 2;
        set.insert(person);
    }
}
