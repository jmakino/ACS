<?xml version="1.0" encoding="utf-8"?>
<!--  $Id: modules.xsl,v 1.12 2002/04/21 20:57:47 wobblini Exp $     -->
<!--  $Author: wobblini $ -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:fo="http://www.w3.org/1999/XSL/Format"
  version="1.0">

  <xsl:template match="class-module-list">
    <xsl:call-template name="c-or-m-list">
      <xsl:with-param name="label">Classes</xsl:with-param>
      <xsl:with-param name="items" select="Class"/>
    </xsl:call-template>
    <xsl:call-template name="c-or-m-list">
      <xsl:with-param name="label">Modules</xsl:with-param>
      <xsl:with-param name="items" select="Module"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="c-or-m-list">
    <xsl:param name="items"/>
    <xsl:param name="label"/>
    <xsl:call-template name="top-level-cm-list">
      <xsl:with-param name="label" select="$label"/>
      <xsl:with-param name="items" select="$items"/>
    </xsl:call-template>
    <xsl:apply-templates select="$items">
      <xsl:sort select="@name"/>
    </xsl:apply-templates>
    <xsl:call-template name="source-code">
      <xsl:with-param name="label" select="$label"/>
      <xsl:with-param name="method-list" select="$items/contents/method-list"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="classmod-info">
    <fo:block space-before="18pt">
      <xsl:call-template name="labeled-list">
        <xsl:with-param name="label">Defined in</xsl:with-param>
        <xsl:with-param name="items" select="./infiles/infile"/>
      </xsl:call-template>
      <xsl:if test="superclass">
      <xsl:call-template name="labeled-list">
        <xsl:with-param name="label">Superclass</xsl:with-param>
        <xsl:with-param name="items" select="superclass"/>
      </xsl:call-template>
      </xsl:if>
    </fo:block>
  </xsl:template>

  <xsl:template match="included-module-list">
    <xsl:call-template name="labeled-list">
      <xsl:with-param name="label">Included modules</xsl:with-param>
      <xsl:with-param name="items" select="./included-module"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="method-list">
    <xsl:call-template name="labeled-list">
      <xsl:with-param name="label">Methods</xsl:with-param>
    </xsl:call-template>
    <xsl:apply-templates select="method"/>
  </xsl:template>

  <xsl:template name="top-level-cm-list">
    <xsl:param name="label"/>
    <xsl:param name="items"/>
    <fo:block break-before="page" break-after="page">
      <fo:block font-size="18pt" font-family="Helvetica" space-after="18pt">
        <xsl:value-of select="$label"/>
      </fo:block>
      <fo:block font-family="monospace">
        <xsl:for-each select="$items">
          <xsl:sort select="@name"/>
          <fo:block start-indent="18pt">
              <fo:basic-link color="purple">
                <xsl:attribute name="internal-destination">
                  <xsl:value-of select="@id"/>
                </xsl:attribute>
                <xsl:apply-templates select="@name"/>
              </fo:basic-link>
          </fo:block>
        </xsl:for-each>
      </fo:block>
    </fo:block>
  </xsl:template>

  <xsl:template match="method">
    <xsl:call-template name="labeled-list">
      <xsl:with-param name="items" select="."/>
    </xsl:call-template>
    <xsl:if test="description">
      <xsl:apply-templates select="description"/>
    </xsl:if>
  </xsl:template>

  <xsl:template match="methodx">
    <fo:block start-indent="12pt">
      <fo:basic-link color="purple">
        <xsl:attribute name="internal-destination">
          <xsl:value-of select="@href"/>
        </xsl:attribute>
        <fo:inline font-family="monospace">    
        <xsl:value-of select="@name"/>
      </fo:inline>
    </fo:basic-link>
  </fo:block>
  <xsl:if test="description">
    <xsl:apply-templates select="description"/>
  </xsl:if>
</xsl:template>

  <xsl:template match="attribute-list">
    <xsl:variable name="read" select="./attribute[attribute-rw = 'R']"/>
    <xsl:variable name="rw" select="./attribute[attribute-rw = 'W']"/>
    <xsl:if test="$read">
      <xsl:call-template name="labeled-list">
        <xsl:with-param name="label" select="'Attributes (read-only)'"/>
      </xsl:call-template>
      <xsl:apply-templates select="$read">
        <xsl:sort select="@name"/>
      </xsl:apply-templates>
    </xsl:if>
    <xsl:if test="$rw">
      <xsl:call-template name="labeled-list">
        <xsl:with-param name="label" select="'Attributes (read/write)'"/>
      </xsl:call-template>
      <xsl:apply-templates select="$rw">
        <xsl:sort select="@name"/>
      </xsl:apply-templates>
    </xsl:if>
  </xsl:template>

  <xsl:template match="attribute">
    <fo:block start-indent="18pt">
      <fo:block font-family="monospace">    
        <xsl:value-of select="@name"/>
      </fo:block>
      <xsl:if test="description">
        <xsl:apply-templates select="description"/>
      </xsl:if>
    </fo:block>
  </xsl:template>

  <xsl:template match="attribute/description|method/description">
    <fo:block start-indent="12pt" end-indent="48pt" 
      font-size="9pt" font-style="italic">
      <xsl:apply-templates/>
    </fo:block>
  </xsl:template>

</xsl:stylesheet>
