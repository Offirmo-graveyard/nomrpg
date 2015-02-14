### special build settings
#  (usually debug, test, experimental, hacks...)
#  requires : COMPUTER_NAME

# option to disable Wt (when the lib is not available)
SET( DEFAULT_USE_WT "ON" )
#SET( DEFAULT_USE_WT  "OFF" )

option(USE_WT  "(dev option) Activate the use of the Wt lib. Available : ON, OFF."  ${DEFAULT_USE_WT} )
MARK_AS_ADVANCED( USE_WT )
