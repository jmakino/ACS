<?xml version="1.0" encoding="utf-8"?>
<!-- $Id: fcm.xsl,v 1.6 2002/03/20 23:42:42 wobblini Exp $ -->
<!-- $Author: wobblini $ -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:fo="http://www.w3.org/1999/XSL/Format"
  version="1.0">

  <!-- Handling for file, Class, or Module, from their
       respective lists -->

  <xsl:template match="file-list/file|Class|Module">
    <fo:block space-after="24pt">
      <xsl:attribute name="id">
        <xsl:value-of select="@id"/>
      </xsl:attribute>
      <xsl:call-template name="hr"/>
      <fo:inline font-family="monospace" font-weight="bold">
        <xsl:value-of select="@name"/>
      </fo:inline>
      <fo:block start-indent="6pt">
        <xsl:apply-templates select="*[name() != 'description']"/>
        <xsl:apply-templates select="*[name() = 'description']"/>
      </fo:block>
    </fo:block>
  </xsl:template>

  <xsl:template match="description">
    <fo:block space-before="12pt" space-after="6pt" font-weight="bold"
      start-indent="0pt">
      <xsl:call-template name="labeled-list">
        <xsl:with-param name="label">Description</xsl:with-param>
        <xsl:with-param name="style">normal</xsl:with-param>
      </xsl:call-template>
    </fo:block>
    <fo:block margin-right="36pt" font-size="9pt"
      text-align="justify">
      <xsl:apply-templates/>
    </fo:block>
  </xsl:template>

  <xsl:template match="file/contents">
    <fo:block start-indent="0pt">
      <xsl:apply-templates/>
    </fo:block>
  </xsl:template>

  <xsl:template match="Class/contents|Module/contents">
    <fo:block>
      <xsl:apply-templates/>
    </fo:block>
  </xsl:template>

</xsl:stylesheet>
