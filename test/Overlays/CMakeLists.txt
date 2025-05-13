cet_test(DTC_Event_t USE_BOOST_UNIT INSTALL_EXAMPLE INSTALL_BIN
    DATAFILES event_test.bin event_test_long.bin event_test_short.bin
    LIBRARIES PRIVATE
    artdaq-core-mu2e::Overlays
)
cet_test(DTC_SubEvent_t USE_BOOST_UNIT INSTALL_EXAMPLE INSTALL_BIN
    DATAFILES subevent_test.bin subevent_test_long.bin subevent_test_short.bin
    LIBRARIES PRIVATE
    artdaq-core-mu2e::Overlays
)
