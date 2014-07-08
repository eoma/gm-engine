
template<class PropertyType>
PropertyListIndexValue<PropertyType>::PropertyListIndexValue(std::shared_ptr<PropertyListData<PropertyType>> data, unsigned int index)
	: data(data), index(index)
{
}

template<class PropertyType>
const PropertyType &PropertyListIndexValue<PropertyType>::get() const 
{ 
	return data->value[index]; 
}

template<class PropertyType>
PropertyType &PropertyListIndexValue<PropertyType>::get() 
{ 
	return data->value[index]; 
}

template<class PropertyType>
void PropertyListIndexValue<PropertyType>::set(const PropertyType &rhs, bool invoke_value_changed)
{
	PropertyType old_value = data->value[index];
	data->value[index] = rhs;
	data->dirty = true; 
	if(invoke_value_changed)
		data->value_changed(index, old_value, rhs);
}

template<class PropertyType>
void PropertyListIndexValue<PropertyType>::operator= (const PropertyType &rhs) 
{
	set(rhs);
}

template<class PropertyType>
PropertyListIndexValue<PropertyType> &PropertyListIndexValue<PropertyType>::operator= (const PropertyListIndexValue<PropertyType> &rhs)
{
	if(this == &rhs)
		return *this;
	throw clan::Exception("Operation not supported!");
}

template<class PropertyType>
PropertyListIndexValue<PropertyType> &PropertyListIndexValue<PropertyType>::operator+= (const PropertyListIndexValue<PropertyType>& rhs)
{
	if(this == &rhs)
		return *this;

	set(get() + rhs.get());
	return *this;
}

template<class PropertyType>
PropertyListIndexValue<PropertyType> &PropertyListIndexValue<PropertyType>::operator+= (const PropertyType& rhs)
{
	set(get() + rhs);
	return *this;
}

template<class PropertyType>
PropertyListIndexValue<PropertyType> &PropertyListIndexValue<PropertyType>::operator-= (const PropertyListIndexValue<PropertyType>& rhs)
{
	if(this == &rhs)
		return *this;

	set(get() - rhs.get());
	return *this;
}

template<class PropertyType>
PropertyListIndexValue<PropertyType> &PropertyListIndexValue<PropertyType>::operator-= (const PropertyType& rhs)
{
	set(get() - rhs);
	return *this;
}

template<class PropertyType>
PropertyListIndexValue<PropertyType> &PropertyListIndexValue<PropertyType>::operator*= (const PropertyListIndexValue<PropertyType>& rhs)
{
	if(this == &rhs)
		return *this;

	set(get() * rhs.get());
	return *this;
}

template<class PropertyType>
PropertyListIndexValue<PropertyType> &PropertyListIndexValue<PropertyType>::operator*= (const PropertyType& rhs)
{
	set(get() * rhs);
	return *this;
}

template<class PropertyType>
PropertyListIndexValue<PropertyType> &PropertyListIndexValue<PropertyType>::operator/= (const PropertyListIndexValue<PropertyType>& rhs)
{
	if(this == &rhs)
		return *this;

	set(get() / rhs.get());
	return *this;
}

template<class PropertyType>
PropertyListIndexValue<PropertyType> &PropertyListIndexValue<PropertyType>::operator/= (const PropertyType& rhs)
{
	set(get() / rhs);
	return *this;
}

template<class PropertyType>
PropertyList<PropertyType>::PropertyList() 
{
}

template<class PropertyType>
PropertyList<PropertyType>::PropertyList(const PropertyList<PropertyType> &copy)
	: data(copy.data)
{
}

template<class PropertyType>
PropertyList<PropertyType>::PropertyList(const std::string &name)
	: data(std::make_shared<PropertyListData<PropertyType>>())
{
	data->name = name;
	data->dirty = false; 
}

template<class PropertyType>
void PropertyList<PropertyType>::push_back(const PropertyType& value, bool invoke_value_added) 
{ 
	data->value.push_back(value); 

	if(invoke_value_added)
		data->value_added(data->value.size()-1, value);
}

template<class PropertyType>
void PropertyList<PropertyType>::erase(unsigned int index, bool invoke_value_erased)
{
	PropertyType value = data->value[index];
	data->value.erase(data->value.begin()+index);
	if(invoke_value_erased)
		data->value_erased(index, value);
}

template<class PropertyType>
void PropertyList<PropertyType>::clear(bool invoke_values_cleared)
{
	data->value.clear();
	if(invoke_values_cleared)
		data->values_cleared();
}

template<class PropertyType>
unsigned int PropertyList<PropertyType>::size() const 
{ 
	return data->value.size(); 
}

template<class PropertyType>
bool PropertyList<PropertyType>::empty() const
{ 
	return data->value.empty(); 
}

template<class PropertyType>
void PropertyList<PropertyType>::resize(unsigned int size, bool invoke_list_resized)
{
	unsigned int old_size = data->value.size();
	data->value.resize(size);
	if(invoke_list_resized)
		data->list_resized(old_size, size);
}

template<class PropertyType>
void PropertyList<PropertyType>::resize(unsigned int size, const PropertyType &value, bool invoke_list_resized)
{
	unsigned int old_size = data->value.size();
	data->value.resize(size, value);
	if(invoke_list_resized)
		data->list_resized(old_size, size);
}

template<class PropertyType>
PropertyListIndexValue<PropertyType> PropertyList<PropertyType>::at(unsigned int index)
{
	if(index >= data->value.size())
		throw clan::Exception("Index was out of bounds for shared property list");

	return PropertyListIndexValue<PropertyType>(data, index);
}

template<class PropertyType>
unsigned int PropertyList<PropertyType>::get_runtime_type_id() const 
{ 
	return IPropertyList::get_runtime_type_id<PropertyType>(); 
}

template<class PropertyType>
const std::vector<PropertyType> &PropertyList<PropertyType>::get() const 
{ 
	return data->value; 
}

template<class PropertyType>
std::vector<PropertyType> &PropertyList<PropertyType>::get() 
{
	return data->value; 
}

template<class PropertyType>
const std::string &PropertyList<PropertyType>::get_name() const 
{
	return data->name;
}

template<class PropertyType>
bool PropertyList<PropertyType>::is_null() const 
{
	return data == nullptr; 
}

template<class PropertyType>
bool PropertyList<PropertyType>::is_dirty() const 
{
	return data->dirty; 
}

template<class PropertyType>
void PropertyList<PropertyType>::clear_dirty() 
{ 
	data->dirty = false; 
}

template<class PropertyType>
clan::Signal<void(unsigned int, const PropertyType &, const PropertyType &)> &PropertyList<PropertyType>::value_changed() 
{ 
	return data->value_changed; 
}

template<class PropertyType>
clan::Signal<void(unsigned int, const PropertyType &)> &PropertyList<PropertyType>::value_added() 
{
	return data->value_added;
}

template<class PropertyType>
clan::Signal<void(unsigned int, const PropertyType &)> &PropertyList<PropertyType>::value_erased() 
{ 
	return data->value_erased;
}

template<class PropertyType>
clan::Signal<void()> &PropertyList<PropertyType>::values_cleared() 
{ 
	return data->values_cleared; 
}

template<class PropertyType>
clan::Signal<void(unsigned int, unsigned int)> &PropertyList<PropertyType>::list_resized()
{
	return data->list_resized; 
}

template<class PropertyType>
PropertyList<PropertyType> PropertyList<PropertyType>::operator= (const PropertyList<PropertyType>& rhs)
{
	data = rhs.data;
	return *this;
}

template<class PropertyType>
PropertyListIndexValue<PropertyType> PropertyList<PropertyType>::operator[] (const unsigned int& index)
{
	return at(index);
}
