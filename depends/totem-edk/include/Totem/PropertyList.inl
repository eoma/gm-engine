
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
void PropertyListIndexValue<PropertyType>::set(const PropertyType &rhs, bool invokeValueChanged)
{
	PropertyType oldValue = data->value[index];
	data->value[index] = rhs;
	data->dirty = true; 
	if(invokeValueChanged)
		data->valueChanged.invoke(index, oldValue, rhs);
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
	throw std::runtime_error("Operation not supported!");
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
void PropertyList<PropertyType>::push_back(const PropertyType& value, bool invokeValueAdded) 
{ 
	data->value.push_back(value); 

	if(invokeValueAdded)
		data->valueAdded.invoke(data->value.size()-1, value);
}

template<class PropertyType>
void PropertyList<PropertyType>::erase(unsigned int index, bool invokeValueErased)
{
	PropertyType value = data->value[index];
	data->value.erase(data->value.begin()+index);
	if(invokeValueErased)
		data->valueErased.invoke(index, value);
}

template<class PropertyType>
void PropertyList<PropertyType>::clear(bool invokeValuesCleared)
{
	data->value.clear();
	if(invokeValuesCleared)
		data->valuesCleared.invoke();
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
void PropertyList<PropertyType>::resize(unsigned int size, bool invokeListResized)
{
	unsigned int oldSize = data->value.size();
	data->value.resize(size);
	if(invokeListResized)
		data->listResized(oldSize, size);
}

template<class PropertyType>
void PropertyList<PropertyType>::resize(unsigned int size, const PropertyType &value, bool invokeListResized)
{
	unsigned int oldSize = data->value.size();
	data->value.resize(size, value);
	if(invokeListResized)
		data->listResized(oldSize, size);
}

template<class PropertyType>
PropertyListIndexValue<PropertyType> PropertyList<PropertyType>::at(unsigned int index)
{
	if(index >= data->value.size())
		throw std::runtime_error("Index was out of bounds for shared property list");

	return PropertyListIndexValue<PropertyType>(data, index);
}

template<class PropertyType>
unsigned int PropertyList<PropertyType>::getRuntimeTypeId() const 
{ 
	return IPropertyList::getRuntimeTypeId<PropertyType>(); 
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
const std::string &PropertyList<PropertyType>::getName() const 
{
	return data->name;
}

template<class PropertyType>
bool PropertyList<PropertyType>::isNull() const 
{
	return data == nullptr; 
}

template<class PropertyType>
bool PropertyList<PropertyType>::isDirty() const 
{
	return data->dirty; 
}

template<class PropertyType>
void PropertyList<PropertyType>::clearDirty() 
{ 
	data->dirty = false; 
}

template<class PropertyType>
sigslot::signal<unsigned int, const PropertyType &, const PropertyType &> &PropertyList<PropertyType>::valueChanged() 
{ 
	return data->valueChanged; 
}

template<class PropertyType>
sigslot::signal<unsigned int, const PropertyType &> &PropertyList<PropertyType>::valueAdded() 
{
	return data->valueAdded;
}

template<class PropertyType>
sigslot::signal<unsigned int, const PropertyType &> &PropertyList<PropertyType>::valueErased() 
{ 
	return data->valueErased;
}

template<class PropertyType>
sigslot::signal<> &PropertyList<PropertyType>::valuesCleared() 
{ 
	return data->valuesCleared; 
}

template<class PropertyType>
sigslot::signal<unsigned int, unsigned int> &PropertyList<PropertyType>::listResized()
{
	return data->listResized; 
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
