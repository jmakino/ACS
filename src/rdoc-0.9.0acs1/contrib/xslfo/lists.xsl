<?xml version="1.0" encoding="utf-8"?>
<!-- $Id: lists.xsl,v 1.3 2002/04/21 20:57:47 wobblini Exp $     -->
<!-- $Author: wobblini $ -->

<xsl:stylesheet
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
  xmlns:fo="http://www.w3.org/1999/XSL/Format">

  <xsl:template match="ul">
    <fo:block start-indent="0pt">
      <fo:list-block margin-left="18pt" space-after="12pt" margin-right="36pt"
        provisional-distance-between-starts="18pt"
        provisional-label-separation="9pt">
        <xsl:apply-templates select="li"/>
      </fo:list-block>
    </fo:block>
  </xsl:template>
  
  <xsl:template match="method/description/ul">
    <fo:block start-indent="0pt">
      <fo:list-block margin-left="48pt" space-after="12pt" margin-right="36pt"
        provisional-distance-between-starts="18pt"
        provisional-label-separation="9pt">
        <xsl:apply-templates select="li"/>
      </fo:list-block>
    </fo:block>
  </xsl:template>
  
  <xsl:template match="li">
    <fo:list-item>
      <fo:list-item-label end-indent="label-end()">
        <fo:block text-align="start">
          <xsl:text>&#x2022;</xsl:text>
        </fo:block>
      </fo:list-item-label>
      <fo:list-item-body>
        <fo:block font-weight="normal" space-after="3pt">
          <xsl:apply-templates/>
        </fo:block>
      </fo:list-item-body>
    </fo:list-item>
  </xsl:template>
  
</xsl:stylesheet>
