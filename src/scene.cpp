#include "scene.hpp"
#include <iostream>

SceneNode::SceneNode(const std::string& name)
  : m_name(name)
{
}

SceneNode::~SceneNode()
{
}

void SceneNode::rotate(char axis, double angle)
{
//	std::cerr << "Stub: Rotate " << m_name << " around " << axis << " by " << angle << std::endl;
	Matrix4x4 temp;
	double radAngle = angle * M_PI / 180.0;
	if (axis == 'x')
	{
		rotX += angle;
		temp[1][2] = -1 * sin(radAngle);
		temp[1][1] = cos(radAngle);
		temp[2][2] = cos(radAngle);
		temp[2][1] = sin(radAngle);
	}
	else if (axis == 'y')
	{
		rotY += angle;
		temp[2][0] = -1 * sin(radAngle);
		temp[0][0] = cos(radAngle);
		temp[2][2] = cos(radAngle);
		temp[0][2] = sin(radAngle);
	}
	else if (axis == 'z')
	{
		rotZ += angle;
		temp[0][1] = -1 * sin(radAngle);
		temp[0][0] = cos(radAngle);
		temp[1][1] = cos(radAngle);
		temp[1][0] = sin(radAngle);
	}
	set_transform(m_trans * temp);
//	m_trans = m_trans * temp;
}

void SceneNode::scale(const Vector3D& amount)
{
	Matrix4x4 temp;
	temp[0][0] = amount[0];
	temp[1][1] = amount[1];
	temp[2][2] = amount[2];
	set_transform(m_trans * temp);
	
	//m_trans = m_trans * temp;
}

void SceneNode::translate(const Vector3D& amount)
{
	// Fill me in*/
	Matrix4x4 temp;
	temp[0][3] += amount[0];
	temp[1][3] += amount[1];
	temp[2][3] += amount[2];
	set_transform(m_trans * temp);
	std::cout << "translate:" << m_trans << "\n";
	//m_trans = m_trans * temp;
}

bool SceneNode::is_joint() const
{
  return false;
}

bool SceneNode::hit(Ray* ray, double epsilon)
{
	ChildList temp = m_children;
	ChildList::iterator i;
	// Transform ray
	// std::cout << "Ori\t" << ray->dir << std::endl;
	ray->origin = m_invtrans * ray->origin;
	Vector3D backupDir = ray->dir;
	ray->dir = m_invtrans * ray->dir;
	// double d = ray->dir.normalize();
	// std::cout << ray->dir << std::endl;
	
	
	for ( i = temp.begin() ; i != temp.end(); i++ )
	{
		(*i)->hit(ray, epsilon);
	}

	// Restore ray
	ray->origin = m_trans * ray->origin;
	ray->dir = m_trans * ray->dir;
	// ray->dir.normalize();
	ray->n = m_invtrans.transpose() * ray->n;
	ray->n.normalize();

	return ray->isHit();	
}

JointNode::JointNode(const std::string& name)
  : SceneNode(name)
{
}

JointNode::~JointNode()
{
}

bool JointNode::is_joint() const
{
  return true;
}

void JointNode::set_joint_x(double min, double init, double max)
{
  m_joint_x.min = min;
  m_joint_x.init = init;
  m_joint_x.max = max;
}

void JointNode::set_joint_y(double min, double init, double max)
{
  m_joint_y.min = min;
  m_joint_y.init = init;
  m_joint_y.max = max;
}

bool JointNode::hit(Ray *ray, double epsilon)
{
	return false;
}

GeometryNode::GeometryNode(const std::string& name, Primitive* primitive)
  : SceneNode(name),
    m_primitive(primitive)
{
}

GeometryNode::~GeometryNode()
{
}
 
bool GeometryNode::hit(Ray *ray, double epsilon)
{
	ChildList temp = m_children;
	ChildList::iterator i;
	
	// Transform ray
	// std::cout << "Ori\t" << ray->dir << std::endl;
	ray->origin = m_invtrans * ray->origin;
	Vector3D backupDir = ray->dir;
	ray->dir = m_invtrans * ray->dir;
	// double d = ray->dir.normalize();
	// std::cout << ray->dir << std::endl;
	
	// iterate through children and intersect with children
	for ( i = temp.begin() ; i != temp.end(); i++ )
	{
		(*i)->hit(ray, epsilon);
	}
	
	// Intersect with primitive
	bool ret = m_primitive->hit(*ray, epsilon);
	
	// Restore ray
	ray->origin = m_trans * ray->origin;
	ray->dir = m_trans * ray->dir;
	// ray->dir.normalize();
	ray->n = m_invtrans.transpose() * ray->n;
	ray->n.normalize();
	
	if (ret)
	{
	//	std::cout << "Hiyo\n";
		ray->material = m_material;
		ray->name = m_name;
	}
	
	
	return ret;
}