<?xml version="1.0" encoding="utf-8"?>
<!-- $Id: utils.xsl,v 1.1 2002/03/21 01:18:51 wobblini Exp $ -->
<!-- $Author: wobblini $ -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:fo="http://www.w3.org/1999/XSL/Format"
  version="1.0">

  <xsl:template name="no-hash-mark">
    <xsl:param name="string"/>
    <xsl:choose>
      <xsl:when test="substring($string,1,1) = '#'">
        <xsl:value-of select="substring($string,2)"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$string"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

</xsl:stylesheet>