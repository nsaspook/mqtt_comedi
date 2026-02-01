/** \file bmcdio.h
 * File:   bmcdio.h
 * Author: root
 *
 * Created on May 12, 2025, 12:57 PM
 */

#ifndef BMCDIO_H
#define	BMCDIO_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "mconfig.h"
#include "tic12400.h"
#include "mc33996.h"

#define BMCDIO_DRIVER "V0.8"

	void SPI_EADOG(void);
	void SPI_TIC12400(void);
	void SPI_MC33996(void);
	void SPI2_PI(void);

#ifdef	__cplusplus
}
#endif

#endif	/* BMCDIO_H */

