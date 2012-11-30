### includes

## third-party
if(${USE_WT})
	INCLUDE_DIRECTORIES( SYSTEM ${Wt_INCLUDE_DIR})
endif()
INCLUDE_DIRECTORIES( SYSTEM ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${LIBUNITTESTPP_INCLUDE_DIRS})
## RW
#INCLUDE_DIRECTORIES(/Soft/infinityFO/linux/RogueWave/SourcePro/Ed11)
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_RWCONFIG_12d") # needed by RW
## Infinity
#INCLUDE_DIRECTORIES( $ENV{INF_ROOT}/src/include/common )

## our app
INCLUDE_DIRECTORIES(app)
INCLUDE_DIRECTORIES(app/helpers)
INCLUDE_DIRECTORIES(app/helpers/wt_related)
INCLUDE_DIRECTORIES(app/models)
INCLUDE_DIRECTORIES(app/models/base)
INCLUDE_DIRECTORIES(app/models/distributed_accountancy)
INCLUDE_DIRECTORIES(app/models/user-society)
INCLUDE_DIRECTORIES(app/models/virtual_world)
INCLUDE_DIRECTORIES(app/models/wt_related) # not our code...
INCLUDE_DIRECTORIES(app/models/the_boring_rpg)
INCLUDE_DIRECTORIES(app/models/avatar)
INCLUDE_DIRECTORIES(app/views)
INCLUDE_DIRECTORIES(app/views/wt)
INCLUDE_DIRECTORIES(app/views/wt/pages)
INCLUDE_DIRECTORIES(app/views/wt/widgets)
INCLUDE_DIRECTORIES(shared)
INCLUDE_DIRECTORIES(generated)
INCLUDE_DIRECTORIES(spec/spec_utils)
INCLUDE_DIRECTORIES(spec/spec_utils/wt_related)
