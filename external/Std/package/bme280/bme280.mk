BME280_SOURCE = bme280.tar.gz
BME280_SITE = https://github.com/andreiva/raspberry-pi-bme280

define BME280_BUILD_CMDS
	$(TARGET_CONFIGURE_OPTS) $(MAKE) -C $(@D)
endef

define BME280_INSTALL_TARGET_CMDS
	$(TARGET_CONFIGURE_OPTS) $(MAKE) -C $(@D) DESTDIR="$(TARGET_DIR)" install
endef

$(eval $(generic-package))

