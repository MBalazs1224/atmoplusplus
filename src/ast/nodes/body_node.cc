#include "body_node.hh"


BodyNode::BodyNode(BodyNode&& other) : statements(std::move(other.statements))
{}