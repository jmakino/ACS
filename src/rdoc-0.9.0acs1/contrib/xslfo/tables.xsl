<?xml version="1.0" encoding="utf-8"?>
<!-- $Id: tables.xsl,v 1.4 2002/03/11 22:12:06 wobblini Exp $     -->
<!-- $Author: wobblini $ -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:fo="http://www.w3.org/1999/XSL/Format"
  version="1.0">

  <xsl:template match="table">
    <fo:block space-after="1em">
      <fo:table table-layout="fixed">
        <fo:table-column column-number="1" column-width="1in"/>
        <fo:table-column column-number="2" column-width="5.5in"/>
        <fo:table-body font-size="10pt">
        <xsl:apply-templates/>
        </fo:table-body>
      </fo:table>
    </fo:block>
  </xsl:template>

  <xsl:template match="dl">
    <fo:block space-before="12pt" space-after="12pt" margin-left="0in">
      <xsl:attribute name="start-indent">
        <xsl:choose>
          <xsl:when test="ancestor::ul">54pt</xsl:when>
          <xsl:otherwise>18pt</xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
      <fo:table table-layout="fixed">
        <fo:table-column column-number="1" column-width="1in"/>
        <fo:table-column column-number="2" column-width="5.5in"/>
        <fo:table-body>
        <xsl:apply-templates select="dt"/>
        </fo:table-body>
      </fo:table>
    </fo:block>
  </xsl:template>

  <xsl:template match="dt">
    <xsl:variable name="pos" select="position()"/>
    <fo:table-row>
      <fo:table-cell text-align="justify">
        <fo:block>
          <fo:inline>
            <xsl:apply-templates/>
          </fo:inline>
        </fo:block>
      </fo:table-cell>    
      <xsl:apply-templates select="../dd[position() = $pos]"/>
    </fo:table-row>
  </xsl:template>

  <xsl:template match="tr">
    <fo:table-row>
      <xsl:apply-templates/>
    </fo:table-row>
  </xsl:template>

  <xsl:template match="td|dd">
    <fo:table-cell text-align="justify">
      <fo:block>
        <xsl:apply-templates/>
      </fo:block>
    </fo:table-cell>    
  </xsl:template>

</xsl:stylesheet>
