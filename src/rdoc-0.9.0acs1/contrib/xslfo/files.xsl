<?xml version="1.0" encoding="utf-8"?>
<!-- $Id: files.xsl,v 1.10 2002/03/20 23:42:42 wobblini Exp $     -->
<!-- $Author: wobblini $ -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:fo="http://www.w3.org/1999/XSL/Format"
  version="1.0">

  <xsl:template match="file-list">
    <xsl:call-template name="top-level-file-list">
      <xsl:with-param name="files" select="./file"/>
    </xsl:call-template>
      <xsl:apply-templates select="file">
      <xsl:sort select="@name"/>
    </xsl:apply-templates>
  </xsl:template>
  
  <xsl:template match="file-info">
    <fo:block space-before="18pt" start-indent="0pt">
      <xsl:call-template name="labeled-list">
        <xsl:with-param name="label">Created</xsl:with-param>
        <xsl:with-param name="items" select="dtm-created"/>
      </xsl:call-template>
      <xsl:call-template name="labeled-list">
        <xsl:with-param name="label">Modified</xsl:with-param>
        <xsl:with-param name="items" select="dtm-modified"/>
      </xsl:call-template>
    </fo:block>
  </xsl:template>

  <xsl:template match="required-file-list">
    <xsl:call-template name="labeled-list">
      <xsl:with-param name="label">Required files</xsl:with-param>
      <xsl:with-param name="items" select="required-file"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="top-level-file-list">
    <xsl:param name="files"/>
    <fo:block break-before="page" break-after="page">
      <fo:block font-size="18pt" font-family="Helvetica" space-after="18pt">
        <xsl:text>Files</xsl:text>
      </fo:block>
      <fo:block font-family="monospace">
        <xsl:for-each select="$files">
          <xsl:sort select="@name"/>
          <fo:block start-indent="18pt">
            <fo:inline>
              <fo:basic-link color="purple">
                <xsl:attribute name="internal-destination">
                  <xsl:value-of select="@id"/>
                </xsl:attribute>
                <xsl:apply-templates select="@name"/>
              </fo:basic-link>
            </fo:inline>
          </fo:block>
        </xsl:for-each>
      </fo:block>
    </fo:block>
  </xsl:template>

</xsl:stylesheet>
