#pragma once
#include "pch.h"

namespace reflect 
{

	//--------------------------------------------------------
	// Base class of all type descriptors
	//--------------------------------------------------------

	struct TypeDescriptor 
	{
		const char* name;
		size_t size;

		TypeDescriptor(const char* name, size_t size) : name{ name }, size{ size } {}
		virtual ~TypeDescriptor() {}
		virtual std::string getFullName() const { return name; }
		virtual void dump(const void* obj, int indentLevel = 0) const = 0;
	};

	//--------------------------------------------------------
	// Finding type descriptors
	//--------------------------------------------------------

	// Declare the function template that handles primitive types such as int, std::string, etc.:
	template <typename T>
	TypeDescriptor* getPrimitiveDescriptor();

	// A helper class to find TypeDescriptors in different ways:
	struct DefaultResolver {
		template <typename T> static char func(decltype(&T::Reflection));
		template <typename T> static int func(...);
		template <typename T>
		struct IsReflected {
			enum { value = (sizeof(func<T>(nullptr)) == sizeof(char)) };
		};

		// This version is called if T has a static member named "Reflection":
		template <typename T, typename std::enable_if<IsReflected<T>::value, int>::type = 0>
		static TypeDescriptor* get() {
			return &T::Reflection;
		}

		// This version is called otherwise:
		template <typename T, typename std::enable_if<!IsReflected<T>::value, int>::type = 0>
		static TypeDescriptor* get() {
			return getPrimitiveDescriptor<T>();
		}
	};

	// This is the primary class template for finding all TypeDescriptors:
	template <typename T>
	struct TypeResolver {
		static TypeDescriptor* get() {
			return DefaultResolver::get<T>();
		}
	};

	//--------------------------------------------------------
	// Type descriptors for user-defined structs/classes
	//--------------------------------------------------------

	struct TypeDescriptor_Struct : TypeDescriptor {
		struct Member {
			const char* name;
			size_t offset;
			TypeDescriptor* type;
		};

		std::vector<Member> members;

		TypeDescriptor_Struct(void (*init)(TypeDescriptor_Struct*)) : TypeDescriptor{ nullptr, 0 } {
			init(this);
		}
		TypeDescriptor_Struct(const char* name, size_t size, const std::initializer_list<Member>& init) : TypeDescriptor{ nullptr, 0 }, members{ init } {
		}
		virtual void dump(const void* obj, int indentLevel) const override {
			std::cout << name << " {" << std::endl;
			for (const Member& member : members) {
				std::cout << std::string(4 * (indentLevel + 1), ' ') << member.name << " = ";
				member.type->dump((char*)obj + member.offset, indentLevel + 1);
				std::cout << std::endl;
			}
			std::cout << std::string(4 * indentLevel, ' ') << "}";
		}
	};

#define REFLECT() \
    friend struct reflect::DefaultResolver; \
    static reflect::TypeDescriptor_Struct Reflection; \
    static void initReflection(reflect::TypeDescriptor_Struct*);

#define REFLECT_STRUCT_BEGIN(type) \
    reflect::TypeDescriptor_Struct type::Reflection{type::initReflection}; \
    void type::initReflection(reflect::TypeDescriptor_Struct* typeDesc) { \
        using T = type; \
        typeDesc->name = #type; \
        typeDesc->size = sizeof(T); \
        typeDesc->members = {

#define REFLECT_STRUCT_MEMBER(name) \
            {#name, offsetof(T, name), reflect::TypeResolver<decltype(T::name)>::get()},

#define REFLECT_STRUCT_END() \
        }; \
    }

	//--------------------------------------------------------
	// Type descriptors for std::vector
	//--------------------------------------------------------

	struct TypeDescriptor_StdVector : TypeDescriptor {
		TypeDescriptor* itemType;
		size_t(*getSize)(const void*);
		const void* (*getItem)(const void*, size_t);

		template <typename ItemType>
		TypeDescriptor_StdVector(ItemType*)
			: TypeDescriptor{ "std::vector<>", sizeof(std::vector<ItemType>) },
			itemType{ TypeResolver<ItemType>::get() } {
			getSize = [](const void* vecPtr) -> size_t {
				const auto& vec = *(const std::vector<ItemType>*) vecPtr;
				return vec.size();
				};
			getItem = [](const void* vecPtr, size_t index) -> const void* {
				const auto& vec = *(const std::vector<ItemType>*) vecPtr;
				return &vec[index];
				};
		}
		virtual std::string getFullName() const override {
			return std::string("std::vector<") + itemType->getFullName() + ">";
		}
		virtual void dump(const void* obj, int indentLevel) const override {
			size_t numItems = getSize(obj);
			std::cout << getFullName();
			if (numItems == 0) {
				std::cout << "{}";
			}
			else {
				std::cout << "{" << std::endl;
				for (size_t index = 0; index < numItems; index++) {
					std::cout << std::string(4 * (indentLevel + 1), ' ') << "[" << index << "] ";
					itemType->dump(getItem(obj, index), indentLevel + 1);
					std::cout << std::endl;
				}
				std::cout << std::string(4 * indentLevel, ' ') << "}";
			}
		}
	};

	// Partially specialize TypeResolver<> for std::vectors:
	template <typename T>
	class TypeResolver<std::vector<T>> {
	public:
		static TypeDescriptor* get() {
			static TypeDescriptor_StdVector typeDesc{ (T*) nullptr };
			return &typeDesc;
		}
	};

	//--------------------------------------------------------
	// Type descriptors for std::unique_ptr
	//--------------------------------------------------------

	struct TypeDescriptor_StdUniquePtr : TypeDescriptor {
		TypeDescriptor* targetType;
		const void* (*getTarget)(const void*);

		// Template constructor:
		template <typename TargetType>
		TypeDescriptor_StdUniquePtr(TargetType* /* dummy argument */)
			: TypeDescriptor{ "std::unique_ptr<>", sizeof(std::unique_ptr<TargetType>) },
			targetType{ TypeResolver<TargetType>::get() } {
			getTarget = [](const void* uniquePtrPtr) -> const void* {
				const auto& uniquePtr = *(const std::unique_ptr<TargetType>*) uniquePtrPtr;
				return uniquePtr.get();
				};
		}
		virtual std::string getFullName() const override {
			return std::string("std::unique_ptr<") + targetType->getFullName() + ">";
		}
		virtual void dump(const void* obj, int indentLevel) const override {
			std::cout << getFullName() << "{";
			const void* targetObj = getTarget(obj);
			if (targetObj == nullptr) {
				std::cout << "nullptr";
			}
			else {
				std::cout << std::endl;
				std::cout << std::string(4 * (indentLevel + 1), ' ');
				targetType->dump(targetObj, indentLevel + 1);
				std::cout << std::endl;
				std::cout << std::string(4 * indentLevel, ' ');
			}
			std::cout << "}";
		}
	};

	// Partially specialize TypeResolver<> for std::unique_ptr<>:
	template <typename T>
	class TypeResolver<std::unique_ptr<T>> {
	public:
		static TypeDescriptor* get() {
			static TypeDescriptor_StdUniquePtr typeDesc{ (T*) nullptr };
			return &typeDesc;
		}
	};

	//--------------------------------------------------------
	// A type descriptor for int
	//--------------------------------------------------------

	struct TypeDescriptor_Int : TypeDescriptor {
		TypeDescriptor_Int() : TypeDescriptor{ "int", sizeof(int) } {
		}
		virtual void dump(const void* obj, int /* unused */) const override {
			std::cout << "int{" << *(const int*)obj << "}";
		}
	};

	template <>
	TypeDescriptor* getPrimitiveDescriptor<int>() {
		static TypeDescriptor_Int typeDesc;
		return &typeDesc;
	}

	//--------------------------------------------------------
	// A type descriptor for std::string
	//--------------------------------------------------------

	struct TypeDescriptor_StdString : TypeDescriptor {
		TypeDescriptor_StdString() : TypeDescriptor{ "std::string", sizeof(std::string) } {
		}
		virtual void dump(const void* obj, int /* unused */) const override {
			std::cout << "std::string{\"" << *(const std::string*)obj << "\"}";
		}
	};

	template <>
	TypeDescriptor* getPrimitiveDescriptor<std::string>() {
		static TypeDescriptor_StdString typeDesc;
		return &typeDesc;
	}

	//--------------------------------------------------------
	// A type descriptor for float
	//--------------------------------------------------------

	struct TypeDescriptor_Float : TypeDescriptor {
		TypeDescriptor_Float() : TypeDescriptor{ "float", sizeof(float) } {
		}
		virtual void dump(const void* obj, int /* unused */) const override {
			std::cout << "float{" << *(const float*)obj << "}";
		}
	};

	template <>
	TypeDescriptor* getPrimitiveDescriptor<float>() {
		static TypeDescriptor_Float typeDesc;
		return &typeDesc;
	}

	//--------------------------------------------------------
	// A type descriptor for double
	//--------------------------------------------------------

	struct TypeDescriptor_Double : TypeDescriptor {
		TypeDescriptor_Double() : TypeDescriptor{ "double", sizeof(double) } {
		}
		virtual void dump(const void* obj, int /* unused */) const override {
			std::cout << "double{" << *(const double*)obj << "}";
		}
	};

	template <>
	TypeDescriptor* getPrimitiveDescriptor<double>() {
		static TypeDescriptor_Double typeDesc;
		return &typeDesc;
	}

	//--------------------------------------------------------
	// A type descriptor for bool
	//--------------------------------------------------------

	struct TypeDescriptor_Bool : TypeDescriptor {
		TypeDescriptor_Bool() : TypeDescriptor{ "bool", sizeof(bool) } {
		}
		virtual void dump(const void* obj, int /* unused */) const override {
			std::cout << "bool{" << *(const bool*)obj << "}";
		}
	};

	template <>
	TypeDescriptor* getPrimitiveDescriptor<bool>() {
		static TypeDescriptor_Bool typeDesc;
		return &typeDesc;
	}

	/// <summary>
	/// A type Descriptor for DirectX::SimpleMath::Vector3
	/// </summary>
	struct TypeDescriptor_Vector3 : TypeDescriptor
	{
		TypeDescriptor_Vector3() : TypeDescriptor{ "Vector3", sizeof(Vector3) }
		{}
		virtual void dump(const void* obj, int) const override
		{
			const Vector3* vec = reinterpret_cast<const Vector3*>(obj);
			std::cout << "Vector3{" << vec->x << ", " << vec->y << ", " << vec->z << "}";
		}
	};

	template <>
	TypeDescriptor* getPrimitiveDescriptor<Vector3>()
	{
		static TypeDescriptor_Vector3 typeDesc;
		return &typeDesc;
	}


} // namespace reflect



/// <summary>
/// Define DirectX::SimpleMath::Vector3 to nlohmann namespace
/// </summary>
namespace nlohmann 
{
	template <>
	struct adl_serializer<DirectX::SimpleMath::Vector3>
	{
		static void to_json(json& j, const DirectX::SimpleMath::Vector3& v)
		{
			j = json{ {"x", v.x}, {"y", v.y}, {"z", v.z} };
		}

		static void from_json(const json& j, DirectX::SimpleMath::Vector3& v)
		{
			j.at("x").get_to(v.x);
			j.at("y").get_to(v.y);
			j.at("z").get_to(v.z);
		}
	};
}

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(DirectX::SimpleMath::Vector3, x, y, z)
