//
// Created by Nicholas Ives on 14/05/2020.
//

#include <test/MockRadarContext.h>

// This actually speeds up compilation of the mocks, as the constructors /
// destructors are the biggest part of the mocks. GMock will build them once
// here

MockRadarContext::MockRadarContext() {}
MockRadarContext::~MockRadarContext() {}
MockRadarState::MockRadarState() {}
MockRadarState::~MockRadarState() {}

