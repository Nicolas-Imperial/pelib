pelib::Algebra
buildAlgebra()
{
pelib::Algebra new_algebra;
std::set<int> set_F;
set_F.insert(1);
set_F.insert(2);
set_F.insert(3);
set_F.insert(4);
set_F.insert(5);
pelib::Set<int> F("F", set_F);
new_algebra.insert(&F);
pelib::Scalar<int> p("p", 4);
new_algebra.insert(&p);
return new_algebra;
}
